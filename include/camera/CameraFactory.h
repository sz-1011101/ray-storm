#ifndef CAMERA_FACTORY_H_
#define CAMERA_FACTORY_H_

#include "camera/AbstractSingleImageCamera.h"
#include "camera/CameraSetup.hpp"
#include "camera/ThinLensCameraSetup.hpp"
#include "utility/RenderedData.h"

namespace ray_storm
{
  namespace camera
  {
    class CameraFactory
    {
    private:

      CameraFactory() {};

    public:

      static CameraSetupPtr createCameraSetup(
        const glm::vec3 &position, 
        const glm::vec3 &lookAt, 
        const glm::vec3 &up,
        float aspectRatio,
        float fov_degrees
      );

      static ThinLensCameraSetupPtr createThinLensCameraSetup(
        const glm::vec3 &position, 
        const glm::vec3 &lookAt, 
        const glm::vec3 &up,
        float aspectRatio,
        float fov_degrees,
        float focalLength,
        float fNumber,
        float focusDistance
      );

      static AbstractSingleImageCameraPtr createPinholeCamera(const CameraSetupPtr &cameraSetup,
        const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height);

      static AbstractSingleImageCameraPtr createThinLensCamera(const ThinLensCameraSetupPtr &cameraSetup,
        const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height);

    };
  }
}

#endif