#ifndef ABSTRACT_CAMERA_H_
#define ABSTRACT_CAMERA_H_

#include <memory>

#include "geometry/Ray.hpp"

namespace ray_storm
{
  namespace camera
  {
    class AbstractCamera
    {

    public:
      
      virtual ~AbstractCamera() {};

      virtual void spawnRay(float x, float y, geometry::Ray &ray) const = 0;

    };

    typedef std::shared_ptr<AbstractCamera> AbstractCameraPtr;
  }
}

#endif