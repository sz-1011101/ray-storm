#include "camera/PinholeCamera.h"

using namespace ray_storm::camera;

PinholeCamera::PinholeCamera(const CameraSetupPtr &cameraSetup, const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height)
  : AbstractSingleImageCamera(renderedData, width, height), cameraSetup(cameraSetup)
{
}

void PinholeCamera::spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper)
{
  const float weight = 1.0f;
  for (RayPackage::SampleRay &sr : rayPackage.rays)
  {
    sr.ray = this->spawnRay(rayPackage.xy);
    sr.weight = weight;
  }
}

ray_storm::geometry::Ray PinholeCamera::spawnRay(const glm::vec2 &xy)
{
  geometry::Ray ray;
  ray.origin = this->cameraSetup->position;
  ray.direction = glm::normalize(this->cameraSetup->forward
    + this->cameraSetup->tanFovHalvedW*this->cameraSetup->right*(xy.x*2.0f - 1.0f)
    + this->cameraSetup->tanFovHalvedH*this->cameraSetup->down*(xy.y*2.0f - 1.0f));
  return ray;
}

void PinholeCamera::gatherSample(const geometry::Ray &ray, const glm::vec3 &sample)
{
  const glm::vec4 o(ray.origin, 1.0f);
  glm::vec4 cs = this->cameraSetup->projMatrix*this->cameraSetup->cameraMatrix*o;
  cs /= cs.w;
  AbstractSingleImageCamera::gatherSample(glm::vec2((-cs.x + 1.0f)/2.0f, (-cs.y + 1.0f)/2.0f), sample);

}

glm::vec3 PinholeCamera::spawnPoint(random::RandomizationHelper &randHelper)
{
  return this->cameraSetup->position;
}