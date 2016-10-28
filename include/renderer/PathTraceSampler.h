#ifndef PATH_TRACE_SAMPLER
#define PATH_TRACE_SAMPLER

#include "renderer/AbstractRadianceSampler.h"

namespace ray_storm
{
  namespace renderer
  {
    class PathTraceSampler : public AbstractRadianceSampler
    {
    public:

      enum METHOD
      {
        NAIVE = 0,
        DIRECT,
        DIRECT_BOUNCE
      };

      PathTraceSampler(METHOD method);

      glm::vec3 sample(
        const scene::ScenePtr &scene,
        const glm::vec3 &position,
        const glm::vec3 &direction,
        random::RandomizationHelper &randHelper
      );

    protected:

      METHOD method;

    private:
      // TODO move these out to seperate samplers?
      glm::vec3 walkPath(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper
      );

      glm::vec3 walkPathDirectLighting(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper
      );

      glm::vec3 walkPathDirectLightingBounce(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper
      );

    };
  }
}

#endif