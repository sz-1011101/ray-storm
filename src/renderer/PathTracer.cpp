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
          radianceSum += this->walkPath(ray, randHelpers[currentThread]);
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

  // we are currently at the camera
  reflected[0] = glm::vec3(1.0f);
  emitted[0] = glm::vec3(0.0f);

  for (uint32_t b = 1; b < maxBounces; b++) // hard limit will bias the result in theory...
  {
    depth = b;
    reflected[b] = glm::vec3(0.0f);
    emitted[b] = glm::vec3(0.0f);

    geometry::Intersection<geometry::Object> intersectY; // this is where will will continue our walk
    if (!this->scene->intersect(ray, intersectY)) //trace ray
    {
      break;
    }

    const glm::vec3 &y = intersectY.intersection.position;
    const glm::vec3 &yN = intersectY.intersection.normal;
    geometry::Object *iObj = intersectY.intersected;
    materials::Material *iMat = intersectY.intersected->getMaterial();

    // current object emittance handling
    emitted[b] = iObj->getEmittance();

    // get a reflection by sampling the bsdf
    random::RandomRay bounceRay;
    if (!iMat->sampleBSDF(ray.direction, y, yN, randHelper, bounceRay))
    {
      break;
    }
    
    glm::vec3 bsdf(0.0f);
    // here we do russian roulette termination
    // and try to evaluate bsdf (together with inverse pdf) 
    // and get next direction
    // l is the direction we are going in next
    // n is the normal of the current intersection
    // v is the direction we are coming from
    if (randHelper.drawUniformRandom() < RUSSIAN_ROULETTE_ALPHA
     && iMat->evaluateBSDF(bounceRay.ray.direction, yN, -ray.direction, bsdf))
    {
      const float rrFactor = 1.0f/RUSSIAN_ROULETTE_ALPHA;
      reflected[b] = bsdf*(1.0f/bounceRay.PDF)*rrFactor;
    }
    else // terminate early
    {
      break;
    }

    // go on with reflected ray
    ray = bounceRay.ray;
  }

  // walk the recusion backwards to accumulate radiance
  glm::vec3 radiance(0.0f);
  for (int b = depth; b >= 0; b--)
  {
    radiance = (emitted[b] + reflected[b]*radiance);
  }

  return radiance;

}

glm::vec3 PathTracer::walkPathDirectLighting(const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper)
{
  return glm::vec3(0.0f);
}