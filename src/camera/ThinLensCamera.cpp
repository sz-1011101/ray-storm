#include "camera/ThinLensCamera.h"

using namespace ray_storm::camera;

ThinLensCamera::ThinLensCamera(const ThinLensCameraSetupPtr &cameraSetup) : cameraSetup(cameraSetup)
{

}

void ThinLensCamera::spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper)
{
  const float weight = 1.0f/rayPackage.size();

  std::vector<glm::vec2> lensPoints;
  this->cameraSetup->lens.spawnPoints(rayPackage.size(), lensPoints, randHelper);

  geometry::Ray mainRay;
  mainRay.origin = glm::vec3(0.0f);
  mainRay.direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)
    + this->cameraSetup->tanFovHalved*glm::vec3(1.0f, 0.0f, 0.0f)*(rayPackage.x*2.0f - 1.0f)
    + this->cameraSetup->tanFovHalved*glm::vec3(0.0f, 1.0f, 0.0f)*(rayPackage.y*2.0f - 1.0f));

  // intersect the main ray (which is never refracted) with the focus plane
  // assume we always intersect
  geometry::Intersection<geometry::Plane> isect;
  this->cameraSetup->focusPlane.intersect(mainRay, isect);

  const glm::vec3 &focusPlanePnt = isect.intersection.position;

  for (std::size_t i = 0; i < rayPackage.size(); i++)
  {
    RayPackage::SampleRay &sr = rayPackage[i];
    glm::vec4 originWS = this->cameraSetup->inverseCameraMatrix*glm::vec4(glm::vec3(lensPoints[i], 0.0f), 1.0f);
    glm::vec4 secPntWS = this->cameraSetup->inverseCameraMatrix*glm::vec4(focusPlanePnt, 1.0f);

    sr.ray.origin = glm::vec3(originWS.x, originWS.y, originWS.z);
    sr.ray.direction = glm::normalize(glm::vec3(secPntWS.x, secPntWS.y, secPntWS.z) - sr.ray.origin);

    sr.weight = weight;
  }
}