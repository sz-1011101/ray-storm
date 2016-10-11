#ifndef PIN_HOLE_CAMERA_H_
#define PIN_HOLE_CAMERA_H_

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"
#include "camera/AbstractCamera.h"
#include "camera/CameraSetup.hpp"

namespace ray_storm
{
  namespace camera
  {
    class PinholeCamera : public AbstractCamera
    {

    public:

      PinholeCamera(const CameraSetup &cameraSetup);

      virtual void spawnRays(RayPackage &rayPackage);

    private:

      CameraSetup cameraSetup;

      geometry::Ray spawnRay(float x, float y);
      
    };
  }

}

#endif