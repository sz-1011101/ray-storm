#include "renderer/PathTraceSampler.h"
#include "dispatchers/EmittanceDispatcher.hpp"
#include "renderer/PathTraceVertexFunctions.hpp"

using namespace ray_storm::renderer;

const float RUSSIAN_ROULETTE_ALPHA = 0.85f;
const uint32_t EXPECTED_BOUNCES = static_cast<uint32_t>(1.0f/(1.0f - RUSSIAN_ROULETTE_ALPHA));

PathTraceSampler::PathTraceSampler(METHOD method)
{
  this->method = method;
}

glm::vec3 PathTraceSampler::sample
(
  const scene::ScenePtr &scene,
  const glm::vec3 &position,
  const glm::vec3 &direction,
  random::RandomizationHelper &randHelper
)
{
  // sample ray goes reverse path!
  geometry::Ray ray(position, -direction);
  switch (this->method)
  {
    case NAIVE:
    return this->naive(scene, ray, randHelper);
    case DIRECT:
    return this->directIllumination(scene, ray, randHelper);
    case DIRECT_BOUNCE:
    return this->directIlluminationBounce(scene, ray, randHelper);
    default:
    return glm::vec3(0.0);
  }
}

void PathTraceSampler::randomWalkEye
(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay,
  random::RandomizationHelper &randHelper,
  RandomWalk &walk
)
{

  walk.vertices.reserve(EXPECTED_BOUNCES);
  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  geometry::Ray ray = initialRay;

  glm::vec3 Lrefl(1.0f);
  bool absorbed = false;
  for (uint32_t b = 0; b < maxBounces; b++) // bounds this loop, biased in theory though
  {
    PathTraceVertex vert;
    if (!PathTraceVertexFunctions::intersect(ray, scene.get(), vert))
    {
      break;
    }

    float rr = (b < 2) ? 1.0f : RUSSIAN_ROULETTE_ALPHA;
    if (PathTraceVertexFunctions::isReflecting(vert) &&
        randHelper.drawUniformRandom() < rr &&
        PathTraceVertexFunctions::bounceEye(randHelper, vert))
    {
      Lrefl *= (1.0f/rr)*vert.bsdf/vert.bsdfPDF;
      vert.cummulative = Lrefl;
      ray = geometry::Ray(vert.outPosition, vert.out);
    }
    else
    {
      absorbed = true;
    }
    
    walk.vertices.push_back(vert);
    if (absorbed)
    {
      break;
    }

    if (b == maxBounces - 1)
    {
      absorbed = true;
    }
  }

  walk.absorbed = absorbed;
}

glm::vec3 PathTraceSampler::naive(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay,
  random::RandomizationHelper &randHelper
)
{
  RandomWalk walk;
  this->randomWalkEye(scene, initialRay, randHelper, walk);
  const std::size_t walkLen = walk.vertices.size();

  if (walkLen == 0)
  {
    return scene->sampleSky(initialRay.direction);
  }

  glm::vec3 L = PathTraceVertexFunctions::emittance(walk.vertices[0]);
  for (std::size_t b = 0; b < walkLen - 1; b++)
  {
    L += walk.vertices[b].cummulative*PathTraceVertexFunctions::emittance(walk.vertices[b + 1]);
  }

  if (!walk.absorbed)
  {
    L += walk.vertices[walkLen - 1].cummulative*scene->sampleSky(walk.vertices[walkLen - 1].out);
  }

  return L;

}

glm::vec3 PathTraceSampler::directIllumination(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay,
  random::RandomizationHelper &randHelper
)
{
  RandomWalk walk;
  this->randomWalkEye(scene, initialRay, randHelper, walk);
  const std::size_t walkLen = walk.vertices.size();

  if (walkLen == 0)
  {
    return scene->sampleSky(initialRay.direction);
  }

  glm::vec3 L = PathTraceVertexFunctions::emittance(walk.vertices[0]);
  for (std::size_t b = 0; b < walkLen; b++)
  {
    PathTraceVertex &vert = walk.vertices[b];
    scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
      vert, scene.get(), randHelper);
    glm::vec3 Ld(0.0f); // direct illumination
    if (!lumSample.shadowed)
    {
      Ld = PathTraceVertexFunctions::evaluateBSDF(lumSample.direction, vert, -vert.in)*lumSample.emittance/lumSample.PDF;
      if (b > 0)
      {
        Ld *= walk.vertices[b - 1].cummulative;
      }
    
      L += Ld;
    }
  }

  return L;

}

glm::vec3 PathTraceSampler::directIlluminationBounce(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay,
  random::RandomizationHelper &randHelper
)
{
  // details for the bsdf/luminaire sample weighting 
  // in http://www.cs.cornell.edu/courses/cs6630/2012sp/slides/07pathtr-slides.pdf
  RandomWalk walk;
  this->randomWalkEye(scene, initialRay, randHelper, walk);
  const std::size_t walkLen = walk.vertices.size();

  if (walkLen == 0)
  {
    return scene->sampleSky(initialRay.direction);
  }

  glm::vec3 L = PathTraceVertexFunctions::emittance(walk.vertices[0]);
  for (std::size_t b = 0; b < walkLen; b++)
  {
    PathTraceVertex &vert = walk.vertices[b];
    scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
      vert, scene.get(), randHelper);
    glm::vec3 LdLum(0.0f); // direct illumination
    glm::vec3 LdBounce(0.0f); // direct illumination of bounce
    
    if (!lumSample.shadowed)
    {
      const float bsdfLumPDF = PathTraceVertexFunctions::bsdfPDF(vert.in, vert, lumSample.direction);
      LdLum = PathTraceVertexFunctions::evaluateBSDF(lumSample.direction, vert, -vert.in)*lumSample.emittance/(lumSample.PDF + bsdfLumPDF);
    }

    if (b < walkLen - 1)
    {
      PathTraceVertex &nextVert = walk.vertices[b + 1];
      const float lumBouncePDF = PathTraceVertexFunctions::luminarePDF(vert.position, nextVert, scene.get());
      LdBounce = PathTraceVertexFunctions::emittance(nextVert)*vert.bsdf/(vert.bsdfPDF + lumBouncePDF);
    }
    else if (!walk.absorbed) // last vertex
    {
      LdBounce = scene->sampleSky(vert.out)*vert.bsdf/(vert.bsdfPDF + scene->getSkyPDF());
    }
    glm::vec3 Ld = LdLum + LdBounce;

    if (b > 0)
    {
      Ld *= walk.vertices[b - 1].cummulative;
    }
    L += Ld;

  }

  return L;

}