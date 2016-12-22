#include "camera/PinholeCamera.h"

using namespace ray_storm::camera;

PinholeCamera::PinholeCamera(const CameraSetupPtr &cameraSetup, const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height)
  : AbstractSingleImageCamera(renderedData, width, height), cameraSetup(cameraSetup)
{
}

void PinholeCamera::spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper)
{
  for (SampleRay &sr : rayPackage.rays)
  {
    this->spawnRay(sr, rayPackage.xy);
  }
}

void PinholeCamera::spawnRay(SampleRay &sampleRay, const glm::vec2 &xy)
{
  geometry::Ray ray;
  ray.origin = this->cameraSetup->position;
  ray.direction = glm::normalize(this->cameraSetup->forward
    + this->cameraSetup->tanFovHalvedW*this->cameraSetup->right*(xy.x*2.0f - 1.0f)
    + this->cameraSetup->tanFovHalvedH*this->cameraSetup->down*(xy.y*2.0f - 1.0f));
  sampleRay = SampleRay(ray, xy);
}

bool PinholeCamera::generateRay(const glm::vec3 &point, SampleRay &sampleRay, random::RandomizationHelper &randHelper)
{
  const glm::vec4 o(point, 1.0f);
  glm::vec4 is = this->cameraSetup->projMatrix*this->cameraSetup->cameraMatrix*o;
  is /= is.w;

  if (is.x < 0.0f || is.x >= 1.0f || is.x < 0.0f || is.x >= 1.0f)
  {
    return false;
  }

  sampleRay = SampleRay(geometry::Ray(
    this->cameraSetup->position, glm::normalize(point - this->cameraSetup->position)), glm::vec2(is.x, is.y));
  return true;
}