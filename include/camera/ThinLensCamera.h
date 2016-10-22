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

      ThinLensCamera(const ThinLensCameraSetupPtr &cameraSetup, const utility::RenderedDataPtr &renderedData);

      virtual ~ThinLensCamera() {};

      virtual void spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper);

    protected:

      ThinLensCameraSetupPtr cameraSetup;
      
    };
  }
}

#endif