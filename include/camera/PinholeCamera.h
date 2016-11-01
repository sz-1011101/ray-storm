#ifndef PIN_HOLE_CAMERA_H_
#define PIN_HOLE_CAMERA_H_

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"
#include "camera/AbstractSingleImageCamera.h"
#include "camera/CameraSetup.hpp"

namespace ray_storm
{
  namespace camera
  {
    class PinholeCamera : public AbstractSingleImageCamera
    {

    public:

      PinholeCamera(const CameraSetupPtr &cameraSetup, const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height);

      virtual ~PinholeCamera() {};

      virtual void spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper);

      virtual void gatherSample(const geometry::Ray &ray, const glm::vec3 &sample);

      virtual glm::vec3 spawnPoint(random::RandomizationHelper &randHelper);

    private:

      CameraSetupPtr cameraSetup;

      geometry::Ray spawnRay(const glm::vec2 &xy);

      glm::mat4 projMat;
      
    };
  }

}

#endif