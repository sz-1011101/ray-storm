#ifndef ABSTRACT_CAMERA_H_
#define ABSTRACT_CAMERA_H_

#include <memory>
#include <vector>

#include "utility/common.hpp"
#include "camera/RayPackage.hpp"
#include "random/RandomizationHelper.h"
#include "camera/SampleRay.hpp"

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

      virtual void incrementSampleCnt(const glm::vec2 &xy) = 0;

      virtual bool generateRay(const glm::vec3 &point, SampleRay &sampleRay) = 0;

    };

    typedef std::shared_ptr<AbstractCamera> AbstractCameraPtr;
  }
}

#endif