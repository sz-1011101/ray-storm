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

      virtual bool generateRay(const glm::vec3 &point, SampleRay &sampleRay);

    private:

      CameraSetupPtr cameraSetup;

      void spawnRay(SampleRay &sampleRay, const glm::vec2 &xy);
      
    };
  }

}

#endif