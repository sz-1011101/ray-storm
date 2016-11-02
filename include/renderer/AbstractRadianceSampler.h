#ifndef ABSTRACT_RADIANCE_SAMPLER_H_
#define ABSTRACT_RADIANCE_SAMPLER_H_

#include <memory>

#include "utility/common.hpp"
#include "random/RandomizationHelper.h"
#include "camera/SampleRay.hpp"

namespace ray_storm
{

  namespace scene
  {
    class Scene;
  }

  namespace camera
  {
    class AbstractCamera;
  }

  namespace renderer
  {
    class AbstractRadianceSampler
    {
    public:

      virtual ~AbstractRadianceSampler() {};
      
      virtual void sample(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      ) = 0;

    };

    typedef std::shared_ptr<AbstractRadianceSampler> AbstractRadianceSamplerPtr;
  }
}

#endif