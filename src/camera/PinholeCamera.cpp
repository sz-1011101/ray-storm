#include "camera/PinholeCamera.h"

using namespace ray_storm::camera;

PinholeCamera::PinholeCamera(
  const glm::vec3 &position,
  const glm::vec3 &lookAt,
  const glm::vec3 &up,
  float aspectRatio,
  float fov_degrees
)
{
  this->position = position;
  this->lookAt = lookAt;
  this->aspectRatio = aspectRatio;
  this->fov_degrees = fov_degrees;

  this->tanFovHalved = std::tan(utility::Math::degToRad(this->fov_degrees)/2.0f);

  // build basic camera vectors
  this->forward = glm::normalize(this->lookAt - this->position);
  this->right = glm::normalize(glm::cross(this->forward, up));
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
  ray.origin = this->position;
  ray.direction = glm::normalize(this->forward
    + this->tanFovHalved*this->right*(x*2.0f - 1.0f) + this->tanFovHalved*this->down*(y*2.0f - 1.0f));
  return ray;
}