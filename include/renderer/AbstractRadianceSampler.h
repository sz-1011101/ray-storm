#ifndef ABSTRACT_RADIANCE_SAMPLER_H_
#define ABSTRACT_RADIANCE_SAMPLER_H_

#include <memory>

#include "utility/common.hpp"
#include "random/RandomizationHelper.h"

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
      
      virtual glm::vec3 sample(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const glm::vec3 &position,
        const glm::vec3 &direction,
        random::RandomizationHelper &randHelper
      ) = 0;

    };

    typedef std::shared_ptr<AbstractRadianceSampler> AbstractRadianceSamplerPtr;
  }
}

#endif