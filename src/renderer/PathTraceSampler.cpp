#include "renderer/PathTraceSampler.h"

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

  // current ray
  geometry::Ray ray = initialRay;

  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  int depth = 0;
  glm::vec3 reflected[maxBounces];
  glm::vec3 emitted[maxBounces];

  geometry::Intersection<geometry::Object> intersectX;
  if (!scene->intersect(ray, intersectX))
  {
    return scene->sampleSky(ray);
  }

  for (uint32_t b = 0; b < maxBounces; b++) 
  {
    depth = b;
    reflected[b] = glm::vec3(0.0f);
    emitted[b] = glm::vec3(0.0f);

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

    const float &pdfBSDFBounce = bounceRay.PDF;
    glm::vec3 bounceBSDF = xMat->evaluateBSDF(bounceRay.ray.direction, xN, xUV, -ray.direction);
    emitted[b] = xObj->getEmittance();

    if (glm::all(glm::lessThanEqual(bounceBSDF, glm::vec3(0.0f))))
    {
      break;
    }
    
    // we have the next ray, intersect
    geometry::Intersection<geometry::Object> intersectY;

    if (scene->intersect(bounceRay.ray, intersectY))
    {
      if (randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA) {
        reflected[b] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*bounceBSDF/pdfBSDFBounce;
      }
    }
    else // sky hit
    {
      reflected[b] = scene->sampleSky(bounceRay.ray)*bounceBSDF/pdfBSDFBounce;
      break;
    }

    ray = bounceRay.ray;
    intersectX = intersectY;
  }

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

  // current ray
  geometry::Ray ray = initialRay;

  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  int depth = 0;
  glm::vec3 reflected[maxBounces];
  glm::vec3 direct[maxBounces];

  geometry::Intersection<geometry::Object> intersectX;
  if (!scene->intersect(ray, intersectX))
  {
    return glm::vec3(0.0f);
  }

  const glm::vec3 emittance = intersectX.intersected->getEmittance();

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

    // directly sample the light sources with area form

    scene::Scene::LuminaireSample lumSmpl;
    if (!scene->drawLuminareSample(randHelper, lumSmpl))
    {
      break;
    }
    const glm::vec3 lumSmplDir = glm::normalize(lumSmpl.position - x);

    // compute direct lighting
    geometry::Ray shadowRay(xOffset, lumSmplDir);

    geometry::Intersection<geometry::Object> intersectL;
    if (glm::dot(lumSmplDir, xN) > 0.0f
      && scene->intersect(shadowRay, intersectL)
      && intersectL.intersected == lumSmpl.object
      && glm::distance(intersectL.intersection.position, lumSmpl.position) < 0.001f
    )
    {
      glm::vec3 lightBSDF = xMat->evaluateBSDF(lumSmplDir, xN, xUV, -ray.direction);
      float pdfLumL = scene->getLuminarePDF(intersectL.intersected);

      direct[b] = lightBSDF*intersectL.intersected->getEmittance()
        /(pdfLumL*(std::pow(glm::length(lumSmpl.position - x), 2.0f)
        /dot(-lumSmplDir, intersectL.intersection.normal)));

    }

    glm::vec3 bounceBSDF = xMat->evaluateBSDF(bounceRay.ray.direction, xN, xUV, -ray.direction);

    if (glm::all(glm::lessThanEqual(bounceBSDF, glm::vec3(0.0f))))
    {
      break;
    }
    // we have the next ray, intersect
    geometry::Intersection<geometry::Object> intersectY;
    bool yHit = scene->intersect(bounceRay.ray, intersectY);

    // termination
    if (yHit)
    {
      if (randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA)
      {
        reflected[b] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*bounceBSDF/pdfBSDFBounce;
      }
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

  return emittance + reflectedRadiance;

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
  const glm::vec3 emitted = intersectX.intersected->getEmittance();

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

    glm::vec3 bounceBSDF = xMat->evaluateBSDF(bounceRay.ray.direction, xN, xUV, -ray.direction);

    // directly sample the light sources
    scene::Scene::LuminaireSample lumSmpl;
    const bool lumSampleDrawn = scene->drawLuminareSample(randHelper, lumSmpl);

    const glm::vec3 lumSmplDir = glm::normalize(lumSmpl.position - x);

    // compute direct lighting
    geometry::Ray shadowRay(xOffset, lumSmplDir);
    glm::vec3 reflY = glm::vec3(0.0f);
    glm::vec3 reflL = glm::vec3(0.0f);

    geometry::Intersection<geometry::Object> intersectL;
    if (lumSampleDrawn
      && glm::dot(lumSmplDir, xN) > 0.0f
      && scene->intersect(shadowRay, intersectL)
      && intersectL.intersected == lumSmpl.object
      && glm::distance(intersectL.intersection.position, lumSmpl.position) < 0.001f
    )
    {
      glm::vec3 lightBSDF = xMat->evaluateBSDF(lumSmplDir, xN, xUV, -ray.direction);
      float pdfBSDFLight = 0.0f;
      const float pdfLumL = lumSmpl.PDF;
      if (!xMat->getPDF(ray.direction, xN, xUV, lumSmplDir, pdfBSDFLight))
      {
        break;
      }
      reflL = lightBSDF*intersectL.intersected->getEmittance()
        /(pdfLumL*(std::pow(glm::length(lumSmpl.position - x), 2.0f)
        /dot(-lumSmplDir, intersectL.intersection.normal))
        + pdfBSDFLight);
    }

    // we can get radiance via the y bounce
    if (yHit)
    {
      if (intersectY.intersected->isEmitting())
      {
        geometry::Object *yObj = intersectY.intersected;
        const float pdfLumY = scene->getLuminarePDF(yObj);
        reflY = bounceBSDF*yObj->getEmittance()
          /(pdfLumY*(std::pow(glm::length(intersectY.intersection.position - x), 2.0f)
          /dot(-bounceRay.ray.direction, intersectY.intersection.normal))
          + pdfBSDFBounce);
      }
    }
    else 
    {
      // We "hit" the sky
      // I assume zero pdf for pdfLumY because the sky is never randomly selected as a light source for direct lighting
      // This means the sky acts similar to an object that emitts
      // I'm not sure if this is allowed...
      reflY = (bounceBSDF*scene->sampleSky(bounceRay.ray))/pdfBSDFBounce;
    }

    direct[b] = (reflL + reflY);

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