#include "camera/CameraFactory.h"
#include "camera/PinholeCamera.h"
#include "camera/ThinLensCamera.h"

using namespace ray_storm::camera;

CameraSetupPtr CameraFactory::createCameraSetup(
  const glm::vec3 &position, 
  const glm::vec3 &lookAt, 
  const glm::vec3 &up,
  float aspectRatio,
  float fov_degrees
)
{
  return camera::CameraSetupPtr(new CameraSetup(position, lookAt, up, aspectRatio, fov_degrees));
}

ThinLensCameraSetupPtr CameraFactory::createThinLensCameraSetup(
  const glm::vec3 &position, 
  const glm::vec3 &lookAt, 
  const glm::vec3 &up,
  float aspectRatio,
  float fov_degrees,
  float focalLength,
  float fNumber,
  float focusDistance
)
{
  return camera::ThinLensCameraSetupPtr(new ThinLensCameraSetup(
    position, lookAt, up, aspectRatio, fov_degrees, focalLength, fNumber,
    geometry::Plane(glm::vec3(0.0f, 0.0f, focusDistance), glm::vec3(0.0f, 0.0f, 1.0f))));
}

AbstractSingleImageCameraPtr CameraFactory::createPinholeCamera(const CameraSetupPtr &cameraSetup,
 const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height)
{
  return AbstractSingleImageCameraPtr(new PinholeCamera(cameraSetup, renderedData, width, height));
}

AbstractSingleImageCameraPtr CameraFactory::createThinLensCamera(const ThinLensCameraSetupPtr &cameraSetup,
  const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height)
{
  return AbstractSingleImageCameraPtr(new ThinLensCamera(cameraSetup, renderedData, width, height));
}