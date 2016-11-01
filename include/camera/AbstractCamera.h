#ifndef ABSTRACT_CAMERA_H_
#define ABSTRACT_CAMERA_H_

#include <memory>
#include <vector>

#include "utility/common.hpp"
#include "camera/RayPackage.hpp"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace camera
  {
    class AbstractCamera
    {

    public:
      
      virtual ~AbstractCamera() {};

      virtual void spawnRays(RayPackage &rayPackage, random::RandomizationHelper &randHelper) = 0;

      virtual void gatherSample(const glm::vec2 &xy, const glm::vec3 &sample) = 0;

      virtual void gatherSample(const geometry::Ray &ray, const glm::vec3 &sample) = 0;

      virtual glm::vec3 spawnPoint(random::RandomizationHelper &randHelper) = 0;

    };

    typedef std::shared_ptr<AbstractCamera> AbstractCameraPtr;
  }
}

#endif