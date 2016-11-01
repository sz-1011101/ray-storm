#ifndef THIN_LENS_CAMERA_H_
#define THIN_LENS_CAMERA_H_

#include "camera/AbstractSingleImageCamera.h"
#include "camera/ThinLensCameraSetup.hpp"

namespace ray_storm
{
  namespace camera
  {
    class ThinLensCamera : public AbstractSingleImageCamera
    {
    public:

      ThinLensCamera(const ThinLensCameraSetupPtr &cameraSetup, const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height);

      virtual ~ThinLensCamera() {};

      virtual void spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper);

      virtual void gatherSample(const geometry::Ray &ray, const glm::vec3 &sample);

      virtual glm::vec3 spawnPoint(random::RandomizationHelper &randHelper);

    protected:

      ThinLensCameraSetupPtr cameraSetup;
      
    };
  }
}

#endif