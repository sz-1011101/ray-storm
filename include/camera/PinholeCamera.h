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

      PinholeCamera(const CameraSetupPtr &cameraSetup, const utility::RenderedDataPtr &renderedData);

      virtual ~PinholeCamera() {};

      virtual void spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper);

    private:

      CameraSetupPtr cameraSetup;

      geometry::Ray spawnRay(float x, float y);
      
    };
  }

}

#endif