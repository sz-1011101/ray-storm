#ifndef PIN_HOLE_CAMERA_H_
#define PIN_HOLE_CAMERA_H_

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"
#include "camera/AbstractCamera.h"

namespace ray_storm
{
  namespace camera
  {
    class PinholeCamera : public AbstractCamera
    {

    public:

      PinholeCamera(
          const glm::vec3 &position, 
          const glm::vec3 &lookAt, 
          const glm::vec3 &up,
          float aspectRatio,
          float fov_degrees
        );

      virtual void spawnRays(RayPackage &rayPackage);

    private:

      glm::vec3 position;
      glm::vec3 lookAt;
      glm::vec3 down;
      glm::vec3 right;
      glm::vec3 forward;

      float aspectRatio;
      float fov_degrees;

      float tanFovHalved;

      geometry::Ray spawnRay(float x, float y);
      
    };
  }

}

#endif