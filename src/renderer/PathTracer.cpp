#include <omp.h> // <- best thing since sliced bread!

#include "renderer/PathTracer.h"
#include "random/RandomRay.hpp"
#include "renderer/RenderJobHelper.h"

using namespace ray_storm::renderer;

const float RUSSIAN_ROULETTE_ALPHA = 0.9f;
const uint32_t EXPECTED_BOUNCES = static_cast<uint32_t>(1.0f/(1.0f - RUSSIAN_ROULETTE_ALPHA));
const uint32_t SAMPLES = 500;

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
          radianceSum += walkPath(ray, randHelpers[currentThread]);
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

glm::vec3 PathTracer::walkPath(const geometry::Ray &intialRay, random::RandomizationHelper &randHelper)
{
  geometry::Ray ray = intialRay;
  // path tracing vars
  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  glm::vec3 weights[maxBounces];
  glm::vec3 emittance[maxBounces];

  int depth = 0;
  geometry::Intersection<geometry::Object> intersection;
  
  // light path reverse traversal
  for (uint32_t b = 0; b < maxBounces; b++)
  {
    depth = b;
    // we hit the sky...
    if (!scene->intersect(ray, intersection))
    {
      emittance[b] = this->scene->getSky();
      break;
    }

    geometry::Object *iObj = intersection.intersected;
    geometry::SimpleIntersection iSmpl = intersection.intersection;
    materials::Material *iMat = iObj->getMaterial();

    materials::Material::LightInteraction lightInteraction;
    bool bounceOn = iMat->computeLightInteraction(-ray.direction, iSmpl.position, iSmpl.normal, randHelper, lightInteraction);
    emittance[b] = lightInteraction.emittance;

    // russsian roulette!
    if (!bounceOn || randHelper.drawUniformRandom() > RUSSIAN_ROULETTE_ALPHA)
    {
      break;
    }

    // inverse russian roulette prob times bsdf times inverse PDF times cos weighting
    weights[b] = (1.0f/RUSSIAN_ROULETTE_ALPHA)*lightInteraction.weight;
    ray = lightInteraction.ray;
  }

  // accumulate radiance according to rendering equation
  glm::vec3 radiance = emittance[depth];
  for (int b = depth - 1; b >= 0; b--)
  {
    radiance = (emittance[b] + weights[b]*radiance);
  }

  return radiance;
}