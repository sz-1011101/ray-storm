#include "camera/ThinLensCamera.h"

using namespace ray_storm::camera;

ThinLensCamera::ThinLensCamera(const ThinLensCameraSetupPtr &cameraSetup, const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height)
  : AbstractSingleImageCamera(renderedData, width, height), cameraSetup(cameraSetup)
{

}

void ThinLensCamera::spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper)
{
  std::vector<glm::vec2> lensPoints;
  this->cameraSetup->lens->spawnPoints(rayPackage.size(), lensPoints, randHelper);

  geometry::Ray mainRay;
  mainRay.origin = glm::vec3(0.0f);
  mainRay.direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)
    + this->cameraSetup->tanFovHalvedW*glm::vec3(1.0f, 0.0f, 0.0f)*(rayPackage.xy.x*2.0f - 1.0f)
    + this->cameraSetup->tanFovHalvedH*glm::vec3(0.0f, 1.0f, 0.0f)*(rayPackage.xy.y*2.0f - 1.0f));

  // intersect the main ray (which is never refracted) with the focus plane
  // assume we always intersect
  geometry::Intersection<geometry::Plane> isect;
  this->cameraSetup->focusPlane.intersect(mainRay, isect);

  const glm::vec3 &focusPlanePnt = isect.intersection.position;

  for (std::size_t i = 0; i < rayPackage.size(); i++)
  {
    SampleRay &sr = rayPackage[i];
    glm::vec4 originWS = this->cameraSetup->inverseCameraMatrix*glm::vec4(glm::vec3(lensPoints[i], 0.0f), 1.0f);
    glm::vec4 secPntWS = this->cameraSetup->inverseCameraMatrix*glm::vec4(focusPlanePnt, 1.0f);

    geometry::Ray ray;
    ray.origin = glm::vec3(originWS.x, originWS.y, originWS.z);
    ray.direction = glm::normalize(glm::vec3(secPntWS.x, secPntWS.y, secPntWS.z) - sr.ray.origin);

    sr = SampleRay(ray, rayPackage.xy);
  }
}

bool ThinLensCamera::generateRay(const glm::vec3 &point, SampleRay &sampleRay, random::RandomizationHelper &randHelper)
{
  const glm::vec4 o(point, 1.0f);
  glm::vec4 cs = this->cameraSetup->cameraMatrix*o;
  const glm::vec3 _cs = glm::vec3(cs.x, cs.y, cs.z);

  // Point behind camera? Also reject extremely close points
  if (_cs.z <= 0.01f)
  {
    return false;
  }

  const glm::vec2 lensPoint = this->cameraSetup->lens->spawnPoint(randHelper);
  const glm::vec3 lensPoint_cs(lensPoint, 0.0f);

  geometry::Ray ray_cs(lensPoint_cs, glm::normalize(_cs - lensPoint_cs));
  geometry::Intersection<geometry::Plane> focusPlaneIntersection;
  if (!this->cameraSetup->focusPlane.intersect(ray_cs, focusPlaneIntersection))
  {
    return false;
  }

  // ray projection into image space
  glm::vec4 intersect_is =
    this->cameraSetup->projMatrix*glm::vec4(focusPlaneIntersection.intersection.position, 1.0f);
  intersect_is /= intersect_is.w;

  if (intersect_is.x < 0.0f || intersect_is.x >= 1.0f || intersect_is.x < 0.0f || intersect_is.x >= 1.0f)
  {
    return false;
  }

  const glm::vec4 lensPoint_ws = this->cameraSetup->inverseCameraMatrix*glm::vec4(lensPoint_cs, 1.0f);
  const glm::vec3 _lensPoint_ws = glm::vec3(lensPoint_ws.x, lensPoint_ws.y, lensPoint_ws.z);

  sampleRay = SampleRay(geometry::Ray(_lensPoint_ws, glm::normalize(point - _lensPoint_ws)), glm::vec2(intersect_is.x, intersect_is.y));
  return true;
}