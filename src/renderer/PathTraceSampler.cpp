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
    return this->walkPathDirectLighting2(scene, ray, randHelper);
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
    if (ls.shadowed != true)
    {
      direct[depth] += ls.emittance*vert.computeLuminaireIncomingBSDF()/ls.PDF;
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
    reflectedRadiance = (direct[b] + reflected[b]*reflectedRadiance);
  }

  return reflectedRadiance;
}

glm::vec3 PathTraceSampler::walkPathDirectLighting2(
  const scene::ScenePtr &scene,
  const geometry::Ray &initialRay, 
  random::RandomizationHelper &randHelper
)
{
  // details for the brdf/luminaire sample weighting 
  // in http://www.cs.cornell.edu/courses/cs6630/2012sp/slides/07pathtr-slides.pdf

  // current ray
  geometry::Ray ray = initialRay;
  dispatchers::EmittanceDispatcher ed;

  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  int depth = 0;
  glm::vec3 reflected[maxBounces];
  glm::vec3 direct[maxBounces];

  geometry::Intersection<geometry::Object> intersectX;
  if (!scene->intersect(ray, intersectX))
  {
    return scene->sampleSky(ray);
  }

  
  // emittance of the first intersected object after the camera

  intersectX.intersected->accept(&ed);
  const glm::vec3 emitted = ed.getEmittance();

  // we found our first intersection! lets go on...
  for (uint32_t b = 0; b < maxBounces; b++) // hard limit will bias the result in theory...
  {
    depth = b;
    reflected[b] = glm::vec3(0.0f);
    direct[b] = glm::vec3(0.0f);

    const glm::vec3 &x = intersectX.intersection.position;
    const glm::vec3 &xN = intersectX.intersection.normal;
    const glm::vec2 &xUV = intersectX.intersection.texCoords;
    geometry::Object *xObj = intersectX.intersected;
    materials::Material *xMat = xObj->getMaterial();

    // get a reflection by sampling the bsdf
    random::RandomRay bounceRay;
    if (!xMat->sampleBSDF(ray.direction, x, xN, xUV, randHelper, bounceRay))
    {
      break;
    }
    const glm::vec3 &xOffset = bounceRay.ray.origin;
    const float &pdfBSDFBounce = bounceRay.PDF;

    // we have the next ray, intersect
    geometry::Intersection<geometry::Object> intersectY;
    bool yHit = scene->intersect(bounceRay.ray, intersectY);

    const glm::vec3 bounceBSDF = xMat->evaluateBSDF(bounceRay.ray.direction, xN, xUV, -ray.direction);

    glm::vec3 reflY = glm::vec3(0.0f);
    glm::vec3 reflL = glm::vec3(0.0f);

    // directly sample the light sources
    scene::Scene::LuminaireSample lumSmpl;
    scene->luminaireSample(xOffset, xN, randHelper, lumSmpl);
    if (!lumSmpl.shadowed)
    {
      glm::vec3 lightBSDF = xMat->evaluateBSDF(lumSmpl.direction, xN, xUV, -ray.direction);
      float pdfBSDFLight = 0.0f;
      const float pdfLumY = lumSmpl.PDF;
      if (!xMat->getPDF(ray.direction, xN, xUV, lumSmpl.direction, pdfBSDFLight))
      {
        break;
      }
      reflL = lightBSDF*lumSmpl.emittance/(pdfLumY + pdfBSDFLight);
    }

    // we can get radiance via the y bounce from an emitting object
    if (yHit)
    {
      intersectY.intersected->accept(&ed);
      if (ed.isEmitting())
      {
        geometry::Object *yObj = intersectY.intersected;
        const float pdfLumY = scene->getLuminairePDF(yObj, bounceRay.ray, intersectY.intersection.position, intersectY.intersection.normal);
        yObj->accept(&ed);
        reflY = bounceBSDF*ed.getEmittance()/(pdfLumY + pdfBSDFBounce);
      }
    }
    else // we hit the sky, sample for radiance
    {
      const float pdfSky = scene->getSkyPDF();
      reflY = bounceBSDF*scene->sampleSky(bounceRay.ray)/(pdfSky + pdfBSDFBounce);
    }

    direct[b] = reflL + reflY;

    if (glm::all(glm::lessThanEqual(bounceBSDF, glm::vec3(0.0f))))
    {
      break;
    }

    // termination
    if (yHit && randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA)
    {
      reflected[b] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*bounceBSDF/pdfBSDFBounce;
    }
    else
    {
      break;
    }

    // go on with reflected ray
    ray = bounceRay.ray;
    intersectX = intersectY; // advance one step
  }

  // walk the recusion backwards to accumulate radiance
  glm::vec3 reflectedRadiance(1.0f);
  for (int b = depth; b >= 0; b--)
  {
    reflectedRadiance = (direct[b] + reflected[b]*reflectedRadiance);
  }

  return emitted + reflectedRadiance;

}