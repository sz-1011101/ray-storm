#include "renderer/PathTracer.h"

using namespace ray_storm::renderer;

void PathTracer::render(const scene::Scene &scene, camera::AbstractCameraPtr &camera)
{
  puts("rendering...");
  const uint width = this->renderedData->getWidth();
  const uint height = this->renderedData->getHeight();

  for (uint x = 0; x < width; x++)
  {
    for (uint y = 0; y < height; y++)
    {
      geometry::Ray ray;
      camera->spawnRay(static_cast<float>(x)/width, static_cast<float>(y)/height, ray);

      geometry::Intersection<geometry::Object> intersection;

      // we intersect something?
      if (scene.intersect(ray, intersection))
      {
        this->renderedData->setPixel(x, y, glm::vec3(1, 0, 0));
      }
      else
      {
        this->renderedData->setPixel(x, y, glm::vec3(0, 1, 0));
      }
    }
  }

  this->renderedData->signalChanged();

  puts("done!");
}
