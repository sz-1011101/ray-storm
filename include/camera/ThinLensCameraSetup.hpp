#ifndef THIN_LENS_CAMERA_SETUPS_H_
#define THIN_LENS_CAMERA_SETUPS_H_

#include "camera/CameraSetup.hpp"
#include "geometry/Plane.hpp"
#include "camera/SimpleLens.hpp"

namespace ray_storm
{
  namespace camera
  {
    class ThinLensCameraSetup : public CameraSetup
    {
    public:
      ThinLensCameraSetup(
        const glm::vec3 &position, 
        const glm::vec3 &lookAt, 
        const glm::vec3 &up,
        float aspectRatio,
        float fov_degrees,
        float focalLength,
        float fNumber,
        const geometry::Plane &focusPlane
      ) : CameraSetup(position, lookAt, up, aspectRatio, fov_degrees), focusPlane(focusPlane),
        lens(new SimpleLens(focalLength/fNumber))
      {

      }

      ThinLensCameraSetup(
        const glm::vec3 &position, 
        const glm::vec3 &lookAt, 
        const glm::vec3 &up,
        float aspectRatio,
        float fov_degrees,
        SimpleLensPtr lens,
        const geometry::Plane &focusPlane
      ) : CameraSetup(position, lookAt, up, aspectRatio, fov_degrees), focusPlane(focusPlane),
        lens(lens)
      {

      }

      geometry::Plane focusPlane;
      SimpleLensPtr lens;

    };

    typedef std::shared_ptr<ThinLensCameraSetup> ThinLensCameraSetupPtr;
  }
}

#endif