#include <omp.h> // <- best thing since sliced bread!
#include <vector>

#include "renderer/DefaultRenderer.h"
#include "random/RandomRay.hpp"
#include "renderer/RenderJobHelper.h"

using namespace ray_storm::renderer;

DefaultRenderer::DefaultRenderer(
  const scene::ScenePtr &scene,
  const camera::AbstractCameraPtr &camera,
  const AbstractRadianceSamplerPtr &sampler,
  uint32_t samples
) : scene(scene), camera(camera), sampler(sampler), samples(samples)
{

}

void DefaultRenderer::render()
{
  // TODO: more informative error handling
  if (this->scene == nullptr || this->camera == nullptr || this->renderedData == nullptr)
  {
    return;
  }

  const uint32_t width = this->renderedData->getWidth();
  const uint32_t height = this->renderedData->getHeight();

  const float xSSoffset = (1.0f/width)/3.0f;
  const float ySSoffset = (1.0f/height)/3.0f;

  const int maxThreads = omp_get_max_threads();

  printf("rendering with %d threads...\n", maxThreads);

  RenderJobVector jobs;
  RenderJobHelper::makeRenderJobs(width, height, static_cast<uint32_t>(maxThreads), jobs);

  printf("rendering %zu jobs...\n", jobs.size());

  // every thread should have its own source for random numbers
  std::vector<random::RandomizationHelper> randHelpers(maxThreads);
  // each threads working data
  std::vector<camera::RayPackage> rayPackages(maxThreads, camera::RayPackage(this->samples));

#pragma omp parallel for schedule(dynamic)
  for (std::size_t j = 0; j < jobs.size(); j++)
  {
    RenderJob &job = jobs[j];
    printf("rendering job %u [xy: %u, %u; wh: %u, %u]\n", job.id, job.xOrigin, job.yOrigin, job.width, job.height);

    // current thread id used to id different random engines
    const int currentThread = omp_get_thread_num();
    camera::RayPackage &rp = rayPackages[currentThread];
    for (uint32_t x = 0; x < job.width; x++)
    {
      for (uint32_t y = 0; y < job.height; y++)
      {
        glm::vec3 pxlRadianceSum(0.0f);
        // do some 2x2 supersampling!
        for (uint32_t subX = 1; subX <= 2; subX++)
        {
          for (uint32_t subY = 1; subY <= 2; subY++)
          {
            rp.setup(static_cast<float>(job.xOrigin + x)/width + xSSoffset*subX,
              static_cast<float>(job.yOrigin + y)/height + ySSoffset*subY);
            camera->spawnRays(rp, randHelpers[currentThread]);

            for (camera::RayPackage::SampleRay &sr : rp.rays)
            {
              sr.sample = this->sampler->sample(this->scene, sr.ray.origin, -sr.ray.direction, randHelpers[currentThread]);
            }

            pxlRadianceSum += rp.recombine();
          }
        }

        job.setPixelSRGB(x, y, pxlRadianceSum/4.0f);
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