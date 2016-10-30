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
    case BIDIRECTIONAL:
    return this->bidirectional(scene, ray, randHelper);
    default:
    return glm::vec3(0.0);
  }
}

void PathTraceSampler::randomWalk
(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay,
  random::RandomizationHelper &randHelper,
  RandomWalk &walk,
  PathTraceVertex::DIRECTION direction = PathTraceVertex::DIRECTION::EYE
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
        PathTraceVertexFunctions::bounce(randHelper, vert, direction))
    {
      Lrefl *= (1.0f/rr)*vert.bsdf/vert.bsdfPDF;
      vert.cummulative = Lrefl;
      ray = geometry::Ray(vert.offPosition, vert.out);
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
  this->randomWalk(scene, initialRay, randHelper, walk);
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
  this->randomWalk(scene, initialRay, randHelper, walk);
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
  this->randomWalk(scene, initialRay, randHelper, walk);
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

glm::vec3 PathTraceSampler::bidirectional(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay,
  random::RandomizationHelper &randHelper
)
{
  RandomWalk eyeWalk;
  this->randomWalk(scene, initialRay, randHelper, eyeWalk, PathTraceVertex::DIRECTION::EYE);
  const std::size_t eyeWalkLen = eyeWalk.vertices.size();

  RandomWalk lightWalk;
  scene::Scene::LuminaireRay lumRay;
  scene->sampleLuminaireRay(randHelper, lumRay);

  const glm::vec3 Le = lumRay.emittance/lumRay.randRay.PDF;

  this->randomWalk(scene, lumRay.randRay.ray, randHelper, lightWalk, PathTraceVertex::DIRECTION::LIGHT);
  const std::size_t lightWalkLen = lightWalk.vertices.size();

  glm::vec3 L(0.0f);
  const float pathWeight = 1.0f/((eyeWalkLen + 1)*lightWalkLen);
  int paths = 0;
  for (std::size_t i = 0; i < eyeWalkLen; i++)
  {
    const PathTraceVertex eyeVert = eyeWalk.vertices[i];

    scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
      eyeVert, scene.get(), randHelper);
    glm::vec3 Ld(0.0f); // direct illumination
    if (!lumSample.shadowed)
    {
      Ld = PathTraceVertexFunctions::evaluateBSDF(lumSample.direction, eyeVert, -eyeVert.in)*lumSample.emittance/lumSample.PDF;
      if (i > 0)
      {
        Ld *= eyeWalk.vertices[i - 1].cummulative;
      }
    
      L += Ld;//*pathWeight;
    }

    paths++;

    for (std::size_t j = 0; j < lightWalkLen; j++)
    {
      const PathTraceVertex lightVert = lightWalk.vertices[j];

      if (scene->visible(eyeVert.position, lightVert.position))
      {
        L += Le*this->pathRadiance(eyeWalk, lightWalk, i, j);//*pathWeight;
      }

      paths++;

    }
  }

  return L/static_cast<float>(paths);

}

glm::vec3 PathTraceSampler::pathRadiance(
  const RandomWalk &eyeWalk,
  const RandomWalk &lightWalk,
  int eyeLen,
  int lightLen
)
{
  // See "Accelerating the bidirectional path tracing algorithm using a dedicated intersection processor"
  const PathTraceVertex eyeVert = eyeWalk.vertices[eyeLen];
  const PathTraceVertex lightVert = lightWalk.vertices[lightLen];

  glm::vec3 L(1.0f);

  if (eyeLen > 0)
  {
    L *= eyeWalk.vertices[eyeLen - 1].cummulative;
  }
  if (lightLen > 0)
  {
    L *= lightWalk.vertices[lightLen - 1].cummulative;
  }
  glm::vec3 e2l = lightVert.position - eyeVert.position;
  glm::vec3 e2lNorm = glm::normalize(e2l);
  float e2llen = glm::length(e2l);
  float e2llensquared = e2llen*e2llen;

  float G = std::abs(glm::dot(e2lNorm, eyeVert.normal))*std::abs(glm::dot(-e2lNorm, lightVert.normal)) / e2llensquared;

  L *= PathTraceVertexFunctions::evaluateBSDF(e2lNorm, eyeVert, -eyeVert.in);
  L *= PathTraceVertexFunctions::evaluateBSDF(lightVert.in, lightVert, -e2lNorm);

  L *= G;

  return L;
}