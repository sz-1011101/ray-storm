#ifndef ABSTRACT_CAMERA_H_
#define ABSTRACT_CAMERA_H_

#include <memory>
#include <vector>

#include "utility/common.hpp"
#include "camera/RayPackage.hpp"

namespace ray_storm
{
  namespace camera
  {
    class AbstractCamera
    {

    public:
      
      virtual ~AbstractCamera() {};

      virtual void spawnRays(RayPackage &rayPackage) = 0;

    };

    typedef std::shared_ptr<AbstractCamera> AbstractCameraPtr;
  }
}

#endif