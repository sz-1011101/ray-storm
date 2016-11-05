#include "renderer/PathTraceSampler.h"
#include "dispatchers/EmittanceDispatcher.hpp"
#include "renderer/PathTraceVertexFunctions.hpp"
#include "scene/Scene.h"
#include "camera/AbstractCamera.h"

using namespace ray_storm::renderer;

const float RUSSIAN_ROULETTE_ALPHA = 0.85f;
const uint32_t EXPECTED_BOUNCES = static_cast<uint32_t>(1.0f/(1.0f - RUSSIAN_ROULETTE_ALPHA));

PathTraceSampler::PathTraceSampler(METHOD method)
{
  this->method = method;
}

void PathTraceSampler::sample
(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  switch (this->method)
  {
    case NAIVE:
    this->naive(scene, camera, sampleRay, randHelper);break;
    case DIRECT:
    this->directIllumination(scene, camera, sampleRay, randHelper);break;
    case DIRECT_BOUNCE:
    this->directIlluminationBounce(scene, camera, sampleRay, randHelper);break;
    case BIDIRECTIONAL:
    this->bidirectional(scene, camera, sampleRay, randHelper);break;
    default:
    break;
  }
}

void PathTraceSampler::randomWalk
(
  scene::Scene *scene,
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
    if (!PathTraceVertexFunctions::intersect(ray, scene, vert))
    {
      break;
    }

    float rr = (b < 2) ? 1.0f : RUSSIAN_ROULETTE_ALPHA;
    if (PathTraceVertexFunctions::isReflecting(vert) &&
      PathTraceVertexFunctions::bounce(randHelper, vert) &&
      (vert.delta || randHelper.drawUniformRandom() < rr) &&
      glm::all(glm::greaterThanEqual(vert.bsdf, glm::vec3(0.00001f)))
    )
    { // we do always bounce on in case of dirac delta function...
      Lrefl *= (vert.delta ? vert.bsdf : (1.0f/rr)*vert.bsdf/vert.bsdfPDF);
      vert.cummulative = Lrefl;
      ray = geometry::Ray(vert.offPosition, vert.out);
      walk.vertices.push_back(vert);
    }
    else
    {
      absorbed = true;
      break;
    }

    if (b == maxBounces - 1)
    {
      absorbed = true;
    }
  }

  walk.absorbed = absorbed;
}

void PathTraceSampler::naive(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  RandomWalk walk;
  this->randomWalk(scene, sampleRay.ray, randHelper, walk);
  const int walkLen = static_cast<int>(walk.vertices.size());
  glm::vec3 L(0.0f);

  if (walkLen == 0)
  {
    if (!walk.absorbed)
    {
      L = scene->sampleSky(sampleRay.ray.direction);
    }
  }
  else
  {
    L = PathTraceVertexFunctions::emittance(walk.vertices[0]);
    for (int b = 0; b < walkLen - 1; b++)
    {
      L += walk.vertices[b].cummulative*PathTraceVertexFunctions::emittance(walk.vertices[b + 1]);
    }

    if (!walk.absorbed)
    {
      L += walk.vertices[walkLen - 1].cummulative*scene->sampleSky(walk.vertices[walkLen - 1].out);
    }
  }

  camera->gatherSample(sampleRay.xy, L);
  camera->incrementSampleCnt(sampleRay.xy);

}

void PathTraceSampler::directIllumination(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  RandomWalk walk;
  this->randomWalk(scene, sampleRay.ray, randHelper, walk);
  const int walkLen = static_cast<int>(walk.vertices.size());
  glm::vec3 L(0.0f);
  
  if (walkLen == 0)
  {
    if (!walk.absorbed)
    {
      L = scene->sampleSky(sampleRay.ray.direction);
    }
  }
  else
  {
    L = PathTraceVertexFunctions::emittance(walk.vertices[0]);
    for (int b = 0; b < walkLen; b++)
    {
      PathTraceVertex &vert = walk.vertices[b];

      if (vert.delta)
      {
        continue;
      }

      scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
        vert, scene, randHelper);
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
  }

  camera->gatherSample(sampleRay.xy, L);
  camera->incrementSampleCnt(sampleRay.xy);
}

void PathTraceSampler::directIlluminationBounce(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  // details for the bsdf/luminaire sample weighting 
  // in http://www.cs.cornell.edu/courses/cs6630/2012sp/slides/07pathtr-slides.pdf
  RandomWalk walk;
  this->randomWalk(scene, sampleRay.ray, randHelper, walk);
  const int walkLen = static_cast<int>(walk.vertices.size());

  glm::vec3 L(0.0f);
  
  if (walkLen == 0)
  {
    if (!walk.absorbed)
    {
      L = scene->sampleSky(sampleRay.ray.direction);
    }
  }
  else
  {
    L = PathTraceVertexFunctions::emittance(walk.vertices[0]);
    for (int b = 0; b < walkLen; b++)
    {
      PathTraceVertex &vert = walk.vertices[b];

      glm::vec3 LdLum(0.0f); // direct illumination
      glm::vec3 LdBounce(0.0f); // direct illumination of bounce

      if (!vert.delta)
      {
        scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
          vert, scene, randHelper);
        
        if (!lumSample.shadowed)
        {
          const float bsdfLumPDF = PathTraceVertexFunctions::bsdfPDF(vert.in, vert, lumSample.direction);
          LdLum = PathTraceVertexFunctions::evaluateBSDF(lumSample.direction, vert, -vert.in)*lumSample.emittance/(lumSample.PDF + bsdfLumPDF);
        }
      }

      if (b < walkLen - 1)
      {
        PathTraceVertex &nextVert = walk.vertices[b + 1];
        const float lumBouncePDF = PathTraceVertexFunctions::luminarePDF(vert.position, nextVert, scene);
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
  }

  camera->gatherSample(sampleRay.xy, L);
  camera->incrementSampleCnt(sampleRay.xy);
}

void PathTraceSampler::bidirectional(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  // See "Accelerating the bidirectional path tracing algorithm using a dedicated intersection processor"
  RandomWalk eyeWalk;
  this->randomWalk(scene, sampleRay.ray, randHelper, eyeWalk);
  const int eyeWalkLen = static_cast<int>(eyeWalk.vertices.size());

  if (eyeWalkLen == 0)
  {
    if (!eyeWalk.absorbed)
    {
      camera->gatherSample(sampleRay.xy, scene->sampleSky(sampleRay.ray.direction));
      camera->incrementSampleCnt(sampleRay.xy);
      return;
    }
  } // HACK to get delta-bounce/sky reflection
  else if (eyeWalkLen == 1 && eyeWalk.vertices[0].delta && !eyeWalk.absorbed)
  {
    camera->gatherSample(sampleRay.xy, scene->sampleSky(eyeWalk.vertices[0].out));
  }

  RandomWalk lightWalk;
  scene::Scene::LuminaireRay lumRay;
  glm::vec3 Le(0.0f); // the light source emittance where we start the light path walk

  if (scene->sampleLuminaireRay(randHelper, lumRay))
  {
    Le = lumRay.emittance/lumRay.randRay.PDF;
    this->randomWalk(scene, lumRay.randRay.ray, randHelper, lightWalk);
  }
  
  const int lightWalkLen = static_cast<int>(lightWalk.vertices.size());

  camera::SampleRay srLum;
  if (!lumRay.directional && !lumRay.randRay.delta && camera->generateRay(lumRay.randRay.ray.origin, srLum) && 
    scene->visible(srLum.ray.origin, lumRay.randRay.ray.origin))
  {
    const float lumDis = glm::distance(srLum.ray.origin, lumRay.randRay.ray.origin);
    camera->gatherSample(srLum.xy, lumRay.emittance/(lumDis*lumDis));
    camera->incrementSampleCnt(srLum.xy);
  }

  // get direct lighting contribution
  camera->gatherSample(sampleRay.xy, this->pathDirectLighting(eyeWalk, scene, randHelper));
  // get contribution by combining paths
  camera->gatherSample(sampleRay.xy, this->pathPathCombination(Le, eyeWalk, lightWalk, scene));

  // get direct light bounce contribution
  for (int j = 0; j < lightWalkLen; j++)
  {
    const PathTraceVertex &lightVert = lightWalk.vertices[j];

    if (lightVert.delta)
    {
      continue;
    }

    camera::SampleRay sr;
    if (camera->generateRay(lightVert.offPosition, sr) && scene->visible(sr.ray.origin, lightVert.offPosition))
    {
      glm::vec3 Lc = Le;
      glm::vec3 l2c = sr.ray.origin - lightVert.position;
      const float l2clen = glm::length(l2c);
      const float l2clenSquared = l2clen*l2clen;
      l2c /= l2clen; // normalize

      if (l2clenSquared >= 0.05f)
      {
        if (j > 0)
        {
          Lc *= lightWalk.vertices[j - 1].cummulative;
        }

        Lc *= PathTraceVertexFunctions::evaluateBSDF(-lightVert.in, lightVert, l2c)/l2clenSquared;
        camera->gatherSample(sr.xy, Lc*pathWeighting(0, j + 1));
        camera->incrementSampleCnt(sr.xy);
      }
    }
  }

  camera->incrementSampleCnt(sampleRay.xy);

}

glm::vec3 PathTraceSampler::pathDirectLighting(
  const RandomWalk &eyeWalk,
  scene::Scene *scene,
  random::RandomizationHelper &randHelper)
{
  glm::vec3 L(0.0f);
  const int eyeWalkLen = static_cast<int>(eyeWalk.vertices.size());
  for (int i = 0; i < eyeWalkLen; i++)
  {
    const PathTraceVertex &eyeVert = eyeWalk.vertices[i];

    if (eyeVert.delta)
    {
      continue;
    }

    scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
      eyeVert, scene, randHelper);
      
    if (!lumSample.shadowed)
    {
      glm::vec3 Ld = PathTraceVertexFunctions::evaluateBSDF(lumSample.direction, eyeVert, -eyeVert.in)
        *lumSample.emittance/lumSample.PDF;

      if (i > 0)
      {
        Ld *= eyeWalk.vertices[i - 1].cummulative;
      }
      L += Ld*this->pathWeighting(i + 1, 0);
    }
  }
  return L;
}

glm::vec3 PathTraceSampler::pathPathCombination(
  const glm::vec3 &Le,
  const RandomWalk &eyeWalk,
  const RandomWalk &lightWalk,
  scene::Scene *scene
)
{
  const int eyeWalkLen = static_cast<int>(eyeWalk.vertices.size());
  const int lightWalkLen = static_cast<int>(lightWalk.vertices.size());

  glm::vec3 L(0.0f);

  for (int i = 0; i < eyeWalkLen; i++)
  {
    const PathTraceVertex &eyeVert = eyeWalk.vertices[i];

    if (eyeVert.delta)
    {
      continue;
    }

    for (int j = 0; j < lightWalkLen; j++)
    {
      const PathTraceVertex &lightVert = lightWalk.vertices[j];

      if (!lightVert.delta && scene->visible(eyeVert.offPosition, lightVert.offPosition, lightVert.normal))
      {
        L += Le*this->pathRadiance(eyeWalk, lightWalk, i, j)*pathWeighting(i + 1, j + 1);
      }

    }
  }
  return L;
}

glm::vec3 PathTraceSampler::pathRadiance(
  const RandomWalk &eyeWalk,
  const RandomWalk &lightWalk,
  int eyeLen,
  int lightLen
)
{
  // See "Accelerating the bidirectional path tracing algorithm using a dedicated intersection processor"
  const PathTraceVertex &eyeVert = eyeWalk.vertices[eyeLen];
  const PathTraceVertex &lightVert = lightWalk.vertices[lightLen];

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
  float e2llen = glm::length(e2l);
  float e2llensquared = e2llen*e2llen;
  e2l = glm::normalize(e2l);

  if (e2llensquared < 0.05f)
  {
    return glm::vec3(0.0f);
  }

  float G = std::abs(glm::dot(e2l, eyeVert.normal))*std::abs(glm::dot(-e2l, lightVert.normal))/e2llensquared;

  L *= PathTraceVertexFunctions::evaluateBSDF(e2l, eyeVert, -eyeVert.in);
  L *= G;
  L *= PathTraceVertexFunctions::evaluateBSDF(-lightVert.in, lightVert, -e2l);

  return L;
}

float PathTraceSampler::pathWeighting(int eyeIndex, int lightIndex)
{
  return 1.0f/(1.0f + eyeIndex + lightIndex);
  //return lightIndex == 0 ? 1.0f : 0.0f;
}