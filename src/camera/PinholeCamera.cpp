#include "camera/PinholeCamera.h"

using namespace ray_storm::camera;

PinholeCamera::PinholeCamera(const CameraSetup &cameraSetup) : cameraSetup(cameraSetup)
{
  this->tanFovHalved = std::tan(utility::Math::degToRad(this->cameraSetup.fov_degrees)/2.0f);

  // build basic camera vectors
  this->forward = glm::normalize(this->cameraSetup.lookAt - this->cameraSetup.position);
  this->right = glm::normalize(glm::cross(this->forward, this->cameraSetup.up));
  this->down = glm::normalize(glm::cross(this->forward, this->right));

}

void PinholeCamera::spawnRays(RayPackage &rayPackage)
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
  ray.origin = this->cameraSetup.position;
  ray.direction = glm::normalize(this->forward
    + this->tanFovHalved*this->right*(x*2.0f - 1.0f) + this->tanFovHalved*this->down*(y*2.0f - 1.0f));
  return ray;
}