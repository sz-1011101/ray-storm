#include "renderer/PathTraceSampler.h"
#include "dispatchers/EmittanceDispatcher.hpp"
#include "renderer/PathTraceVertex.hpp"

using namespace ray_storm::renderer;

const float RUSSIAN_ROULETTE_ALPHA = 0.85f;
const uint32_t EXPECTED_BOUNCES = static_cast<uint32_t>(1.0f/(1.0f - RUSSIAN_ROULETTE_ALPHA));

PathTraceSampler::PathTraceSampler(METHOD method)
{
  this->method = method;
}

glm::vec3 PathTraceSampler::sample(
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
    return this->walkPath(scene, ray, randHelper);
    case DIRECT:
    return this->walkPathDirectLighting(scene, ray, randHelper);
    case DIRECT_BOUNCE:
    return this->walkPathDirectLightingBounce(scene, ray, randHelper);
    default:
    return glm::vec3(0.0);
  }
}

glm::vec3 PathTraceSampler::walkPath(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay, 
  random::RandomizationHelper &randHelper
)
{
  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  uint32_t depth = 0;
  PathTraceVertex vertices[maxBounces];
  glm::vec3 reflected[maxBounces];
  glm::vec3 emitted[maxBounces];

  vertices[0] = PathTraceVertex(initialRay);

  for (uint32_t b = 0; b < maxBounces; b++) 
  {
    depth = b;
    reflected[depth] = glm::vec3(0.0f);
    emitted[depth] = glm::vec3(0.0f);
    PathTraceVertex &vert = vertices[depth];

    // find intersection
    if (!vert.computeIntersection(scene.get()))
    {
      emitted[depth] = scene->sampleSky(vert.getIncoming());
      break;
    }

    emitted[depth] = vert.computeEmittance();

    if (!vert.isReflecting() || !vert.computeBounce(randHelper))
    {
      break;
    }

    // eval bsdf
    glm::vec3 bsdf = vert.computeBounceIncomingBSDF();

    if (randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA) {
      reflected[depth] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*bsdf/vert.getBounceIncomingPDF();
    }
    else
    {
      break;
    }

    if (depth < maxBounces - 1)
    {
      vert.setupNext(vertices[depth + 1]);
    }

  }

  // recombine result
  glm::vec3 reflectedRadiance(1.0f);
  for (int b = depth; b >= 0; b--)
  {
    reflectedRadiance = (emitted[b] + reflected[b]*reflectedRadiance);
  }

  return reflectedRadiance;
}

glm::vec3 PathTraceSampler::walkPathDirectLighting(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay, 
  random::RandomizationHelper &randHelper
)
{
  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  uint32_t depth = 0;
  PathTraceVertex vertices[maxBounces];
  glm::vec3 reflected[maxBounces];
  glm::vec3 direct[maxBounces];

  vertices[0] = PathTraceVertex(initialRay);

  for (uint32_t b = 0; b < maxBounces; b++) 
  {
    depth = b;
    reflected[depth] = glm::vec3(0.0f);
    direct[depth] = glm::vec3(0.0f);
    PathTraceVertex &vert = vertices[depth];

    // find intersection
    if (!vert.computeIntersection(scene.get()))
    {
      if (depth == 0) {
        direct[depth] = scene->sampleSky(vert.getIncoming());
      }
      break;
    }

    if (!vert.isReflecting() || !vert.computeBounce(randHelper))
    {
      break;
    }

    // HACK? otherwise emitters are black on first bounce
    if (depth == 0)
    {
      direct[depth] = vert.computeEmittance();
    }

    vert.computeLuminaireSample(scene.get(), randHelper);
    const scene::Scene::LuminaireSample &ls = vert.getLuminaireSample();
    if (!ls.shadowed)
    {
      direct[depth] += ls.emittance*vert.computeLuminaireIncomingBSDF()/ls.PDF;
    }

    // eval bsdf
    glm::vec3 bsdf = vert.computeBounceIncomingBSDF();

    if (randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA)
    {
      reflected[depth] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*bsdf/vert.getBounceIncomingPDF();
    }
    else
    {
      break;
    }

    if (depth < maxBounces - 1)
    {
      vert.setupNext(vertices[depth + 1]);
    }

  }

  // recombine result
  glm::vec3 reflectedRadiance(1.0f);
  for (int b = depth; b >= 0; b--)
  {
    reflectedRadiance = (direct[b] + reflected[b]*reflectedRadiance);
  }

  return reflectedRadiance;
}

glm::vec3 PathTraceSampler::walkPathDirectLightingBounce(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay, 
  random::RandomizationHelper &randHelper
)
{
  // details for the brdf/luminaire sample weighting 
  // in http://www.cs.cornell.edu/courses/cs6630/2012sp/slides/07pathtr-slides.pdf
  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  uint32_t depth = 0;
  PathTraceVertex vertices[maxBounces];
  glm::vec3 reflected[maxBounces];
  glm::vec3 direct[maxBounces];

  vertices[0] = PathTraceVertex(initialRay);
  PathTraceVertex &vert = vertices[depth];
  vert.computeIntersection(scene.get());

  if (!vert.isIntersecting())
  {
    return scene->sampleSky(vert.getIncoming());
  }

  for (uint32_t b = 0; b < maxBounces; b++) 
  {
    depth = b;
    reflected[depth] = glm::vec3(0.0f);
    direct[depth] = glm::vec3(0.0f);
    vert = vertices[depth];

    // HACK evil hack to get emitting emitters...
    if (depth == 0)
    {
      direct[depth] = vert.computeEmittance();
    }

    if (!vert.isReflecting() || !vert.computeBounce(randHelper))
    {
      break;
    }

    // eval bsdf + pdf
    const glm::vec3 bounceBSDF = vert.computeBounceIncomingBSDF();
    const float bouncePDF = vert.getBounceIncomingPDF();

    vert.computeLuminaireSample(scene.get(), randHelper);
    const scene::Scene::LuminaireSample &ls = vert.getLuminaireSample();
    if (!ls.shadowed)
    {
      direct[depth] += ls.emittance*vert.computeLuminaireIncomingBSDF()/(ls.PDF + vert.computePDF_BSDF(vert.getIncoming().direction, ls.direction));
    }

    if (depth < maxBounces - 1)
    {
      PathTraceVertex &nextVert = vertices[depth + 1];
      vert.setupNext(nextVert);

      // bounce illumination
      if (!nextVert.computeIntersection(scene.get()))
      {
        // bounce goes into sky
        direct[depth] += bounceBSDF*scene->sampleSky(vert.getIncoming())/(scene->getSkyPDF() + bouncePDF);
        break;
      }
      else
      {
        // bounce goes into next object
        direct[depth] += bounceBSDF*nextVert.computeEmittance()/(nextVert.computeLuminairePDF(scene.get()) + bouncePDF);
      }
    }

    if (randHelper.drawUniformRandom() >= RUSSIAN_ROULETTE_ALPHA)
    {
      break;
    }
    reflected[depth] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*bounceBSDF/bouncePDF;
  }

  // recombine result
  glm::vec3 reflectedRadiance(1.0f);
  for (int b = depth; b >= 0; b--)
  {
    reflectedRadiance = (direct[b] + reflected[b]*reflectedRadiance);
  }

  return reflectedRadiance;
}