#include <omp.h> // <- best thing since sliced bread!

#include "renderer/PathTracer.h"
#include "random/RandomRay.hpp"
#include "renderer/RenderJobHelper.h"

using namespace ray_storm::renderer;

const uint BOUNCES = 4;
const uint SAMPLES = 128;

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
  glm::vec3 weights[BOUNCES];
  glm::vec3 emittance[BOUNCES];

  int depth = 0;

  geometry::Intersection<geometry::Object> intersection;
  
  // light path reverse traversal
  for (uint b = 0; b < BOUNCES; b++)
  {
    depth = b;
    // we hit the sky...
    if (!scene->intersect(ray, intersection))
    {
      emittance[b] = this->scene->getSky();
      continue;
    }

    geometry::Object *iObj = intersection.intersected;
    geometry::SimpleIntersection iSmpl = intersection.intersection;
    materials::AbstractMaterial *iMat = iObj->getMaterial();

    random::RandomRay randRay;
    iMat->drawReflectedRay(-ray.direction, iSmpl.position + iSmpl.normal*0.001f, iSmpl.normal, randHelper, randRay);
    const glm::vec3 brdf = iMat->evaluateBRDF(-ray.direction, iSmpl.normal, randRay.ray.direction);

    // brdf times inverse PDF times cos weighting
    weights[b] = brdf*randRay.inversePDF*glm::dot(iSmpl.normal, randRay.ray.direction);
    emittance[b] = iMat->getEmittance();

    ray = randRay.ray;
  }

  // accumulate radiance according to rendering equation
  glm::vec3 radiance = emittance[depth];
  for (int b = depth - 1; b >= 0; b--)
  {
    radiance = (emittance[b] + weights[b]*radiance);
  }

  return radiance;
}