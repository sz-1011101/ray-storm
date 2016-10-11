#ifndef ABSTRACT_RADIANCE_SAMPLER_H_
#define ABSTRACT_RADIANCE_SAMPLER_H_

#include <memory>

#include "utility/common.hpp"
#include "scene/Scene.h"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace renderer
  {
    class AbstractRadianceSampler
    {
    public:

      virtual ~AbstractRadianceSampler() {};
      
      virtual glm::vec3 sample(
        const scene::ScenePtr &scene,
        const glm::vec3 &position,
        const glm::vec3 &direction,
        random::RandomizationHelper &randHelper
      ) = 0;

    };

    typedef std::shared_ptr<AbstractRadianceSampler> AbstractRadianceSamplerPtr;
  }
}

#endif