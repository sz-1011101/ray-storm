#include <omp.h> // <- best thing since sliced bread!

#include "renderer/PathTracer.h"
#include "random/RandomRay.hpp"
#include "renderer/RenderJobHelper.h"

using namespace ray_storm::renderer;

const float RUSSIAN_ROULETTE_ALPHA = 0.8f;
const uint32_t EXPECTED_BOUNCES = static_cast<uint32_t>(1.0f/(1.0f - RUSSIAN_ROULETTE_ALPHA));
const uint32_t SAMPLES = 1000;

PathTracer::PathTracer()
{

}

PathTracer::PathTracer(scene::ScenePtr &scene, camera::AbstractCameraPtr &camera) : 
  scene(scene), camera(camera)
{

}

void PathTracer::setScene(scene::ScenePtr &scene)
{
  this->scene = scene;
}

void PathTracer::setCamera(camera::AbstractCameraPtr &camera)
{
  this->camera = camera;
}

void PathTracer::render()
{
  // TODO: more informative error handling
  if (this->scene == nullptr || this->camera == nullptr || this->renderedData == nullptr)
  {
    return;
  }

  const uint32_t width = this->renderedData->getWidth();
  const uint32_t height = this->renderedData->getHeight();

  const int maxThreads = omp_get_max_threads();

  printf("rendering with %d threads...\n", maxThreads);

  RenderJobVector jobs;
  RenderJobHelper::makeRenderJobs(width, height, static_cast<uint32_t>(maxThreads), jobs);

  printf("rendering %zu jobs...\n", jobs.size());

  // every thread should have its own source for random numbers
  random::RandomizationHelper randHelpers[maxThreads];

#pragma omp parallel for schedule(dynamic)
  for (std::size_t j = 0; j < jobs.size(); j++)
  {
    RenderJob &job = jobs[j];
    printf("rendering job %u [xy: %u, %u; wh: %u, %u]\n", job.id, job.xOrigin, job.yOrigin, job.width, job.height);

    // current thread id used to id different random engines
    const int currentThread = omp_get_thread_num();

    for (uint32_t x = 0; x < job.width; x++)
    {
      for (uint32_t y = 0; y < job.height; y++)
      {
        glm::vec3 radianceSum(0.0f);
        geometry::Ray ray;
        camera->spawnRay(static_cast<float>(job.xOrigin + x)/width, static_cast<float>(job.yOrigin + y)/height, ray);

        for (uint32_t s = 0; s < SAMPLES; s++)
        {
          // we can reuse the first ray
          radianceSum += this->walkPathDirectLighting2(ray, randHelpers[currentThread]);
        }

        job.setPixelSRGB(x, y, radianceSum/static_cast<float>(SAMPLES));
      }
    }

    // job done -> update window
    #pragma omp critical
    {
      this->renderedData->setTile(job.xOrigin, job.yOrigin, job.tile);
      this->renderedData->signalChanged();
    }
  }

  puts("done!");
}

glm::vec3 PathTracer::walkPath(const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper)
{

  // current ray
  geometry::Ray ray = initialRay;

  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  int depth = 0;
  glm::vec3 reflected[maxBounces];
  glm::vec3 emitted[maxBounces];

  geometry::Intersection<geometry::Object> intersectX;
  if (!this->scene->intersect(ray, intersectX))
  {
    return glm::vec3(0.0f);
  }

  for (uint32_t b = 0; b < maxBounces; b++) 
  {
    depth = b;
    reflected[b] = glm::vec3(0.0f);
    emitted[b] = glm::vec3(0.0f);

    const glm::vec3 &x = intersectX.intersection.position;
    const glm::vec3 &xN = intersectX.intersection.normal;
    geometry::Object *xObj = intersectX.intersected;
    materials::Material *xMat = xObj->getMaterial();

    // get a reflection by sampling the bsdf
    random::RandomRay bounceRay;
    if (!xMat->sampleBSDF(ray.direction, x, xN, randHelper, bounceRay))
    {
      break;
    }

    const float &pdfBSDFBounce = bounceRay.PDF;

    emitted[b] = xObj->getEmittance();
    
    // we have the next ray, intersect
    geometry::Intersection<geometry::Object> intersectY;
    // termination
    if (this->scene->intersect(bounceRay.ray, intersectY) && randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA)
    {
      glm::vec3 bounceBSDF(0.0f);
      if (!xMat->evaluateBSDF(bounceRay.ray.direction, xN, -ray.direction, bounceBSDF))
      {
        break;
      }
      reflected[b] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*bounceBSDF/pdfBSDFBounce;
    }
    else
    {
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

glm::vec3 PathTracer::walkPathDirectLighting(const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper)
{

  // current ray
  geometry::Ray ray = initialRay;

  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  int depth = 0;
  glm::vec3 reflected[maxBounces];
  glm::vec3 direct[maxBounces];

  geometry::Intersection<geometry::Object> intersectX;
  if (!this->scene->intersect(ray, intersectX))
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
    geometry::Object *xObj = intersectX.intersected;
    materials::Material *xMat = xObj->getMaterial();

    // get a reflection by sampling the bsdf
    random::RandomRay bounceRay;
    if (!xMat->sampleBSDF(ray.direction, x, xN, randHelper, bounceRay))
    {
      break;
    }
    const glm::vec3 &xOffset = bounceRay.ray.origin;
    const float &pdfBSDFBounce = bounceRay.PDF;

    // directly sample the light sources with area form

    scene::Scene::LuminaireSample lumSmpl;
    if (!this->scene->drawLuminareSample(randHelper, lumSmpl))
    {
      break;
    }
    const glm::vec3 lumSmplDir = glm::normalize(lumSmpl.position - x);

    // compute direct lighting
    geometry::Ray shadowRay(xOffset, lumSmplDir);

    geometry::Intersection<geometry::Object> intersectL;
    if (glm::dot(lumSmplDir, xN) > 0.0f
      && this->scene->intersect(shadowRay, intersectL)
      && intersectL.intersected == lumSmpl.object
      && glm::distance(intersectL.intersection.position, lumSmpl.position) < 0.001f
    )
    {
      glm::vec3 lightBSDF(0.0f);
      float pdfLumL = this->scene->getLuminarePDF(intersectL.intersected);
      if (!xMat->evaluateBSDF(lumSmplDir, xN, -ray.direction, lightBSDF))
      {
        break;
      }

      direct[b] = lightBSDF*intersectL.intersected->getEmittance()
        *dot(-lumSmplDir, intersectL.intersection.normal)
        *(1.0f/std::pow(glm::length(lumSmpl.position - x), 2.0f))
        /pdfLumL;
    }

    // we have the next ray, intersect
    geometry::Intersection<geometry::Object> intersectY;
    bool yHit = this->scene->intersect(bounceRay.ray, intersectY);

    // termination
    if (yHit && randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA)
    {
      glm::vec3 bounceBSDF(0.0f);
      if (!xMat->evaluateBSDF(bounceRay.ray.direction, xN, -ray.direction, bounceBSDF))
      {
        break;
      }
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

  return emittance + reflectedRadiance;

}

glm::vec3 PathTracer::walkPathDirectLighting2(const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper)
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
  if (!this->scene->intersect(ray, intersectX))
  {
    return glm::vec3(0.0f);
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
    geometry::Object *xObj = intersectX.intersected;
    materials::Material *xMat = xObj->getMaterial();

    // get a reflection by sampling the bsdf
    random::RandomRay bounceRay;
    if (!xMat->sampleBSDF(ray.direction, x, xN, randHelper, bounceRay))
    {
      break;
    }
    const glm::vec3 &xOffset = bounceRay.ray.origin;
    const float &pdfBSDFBounce = bounceRay.PDF;

    // we have the next ray, intersect
    geometry::Intersection<geometry::Object> intersectY;
    bool yHit = this->scene->intersect(bounceRay.ray, intersectY);

    glm::vec3 bounceBSDF(0.0f);
    if (!xMat->evaluateBSDF(bounceRay.ray.direction, xN, -ray.direction, bounceBSDF))
    {
      break;
    }

    // directly sample the light sources
    scene::Scene::LuminaireSample lumSmpl;
    if (!this->scene->drawLuminareSample(randHelper, lumSmpl))
    {
      break;
    }
    const glm::vec3 lumSmplDir = glm::normalize(lumSmpl.position - x);

    // compute direct lighting
    geometry::Ray shadowRay(xOffset, lumSmplDir);
    glm::vec3 reflY = glm::vec3(0.0f);
    glm::vec3 reflL = glm::vec3(0.0f);

    geometry::Intersection<geometry::Object> intersectL;
    if (glm::dot(lumSmplDir, xN) > 0.0f
      && this->scene->intersect(shadowRay, intersectL)
      && intersectL.intersected == lumSmpl.object
      && glm::distance(intersectL.intersection.position, lumSmpl.position) < 0.001f
    )
    {
      glm::vec3 lightBSDF(0.0f);
      float pdfBSDFLight = 0.0f;
      float pdfLumL = this->scene->getLuminarePDF(intersectL.intersected);
      if (!xMat->evaluateBSDF(lumSmplDir, xN, -ray.direction, lightBSDF)
        || !xMat->getPDF(ray.direction, xN, lumSmplDir, pdfBSDFLight))
      {
        break;
      }
      reflL = lightBSDF*intersectL.intersected->getEmittance()
        *dot(-lumSmplDir, intersectL.intersection.normal)
        *(1.0f/std::pow(glm::length(lumSmpl.position - x), 2.0f))
        /(pdfLumL + pdfBSDFLight);
    }

    // we can get radiance via the y bounce
    if (yHit)
    {
      geometry::Object *yObj = intersectY.intersected;
      const float pdfLumY = this->scene->getLuminarePDF(yObj);
      reflY = bounceBSDF*yObj->getEmittance()/(pdfLumY + pdfBSDFBounce);
    }

    direct[b] = (reflL + reflY);

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