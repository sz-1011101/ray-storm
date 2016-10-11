#include "camera/PinholeCamera.h"

using namespace ray_storm::camera;

PinholeCamera::PinholeCamera(const CameraSetupPtr &cameraSetup) : cameraSetup(cameraSetup)
{

}

void PinholeCamera::spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper)
{
  const float weight = 1.0f/rayPackage.size();
  for (RayPackage::SampleRay &sr : rayPackage.rays)
  {
    sr.ray = this->spawnRay(rayPackage.x, rayPackage.y);
    sr.weight = weight;
  }
}

ray_storm::geometry::Ray PinholeCamera::spawnRay(float x, float y)
{
  geometry::Ray ray;
  ray.origin = this->cameraSetup->position;
  ray.direction = glm::normalize(this->cameraSetup->forward
    + this->cameraSetup->tanFovHalved*this->cameraSetup->right*(x*2.0f - 1.0f)
    + this->cameraSetup->tanFovHalved*this->cameraSetup->down*(y*2.0f - 1.0f));
  return ray;
}