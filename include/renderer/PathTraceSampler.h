#ifndef PATH_TRACE_SAMPLER
#define PATH_TRACE_SAMPLER

#include "renderer/AbstractRadianceSampler.h"
#include "renderer/PathTraceVertex.hpp"

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
        DIRECT_BOUNCE,
        BIDIRECTIONAL
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

      struct RandomWalk
      {
        RandomWalk() : absorbed(true) {}

        bool absorbed;
        std::vector<PathTraceVertex> vertices;
      };

      void randomWalk(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay,
        random::RandomizationHelper &randHelper,
        RandomWalk &walk,
        PathTraceVertex::DIRECTION direction
      );

      // TODO move these out to seperate samplers?
      glm::vec3 naive(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper
      );

      glm::vec3 directIllumination(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper
      );

      glm::vec3 directIlluminationBounce(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay,
        random::RandomizationHelper &randHelper
      );

      glm::vec3 bidirectional(
        const scene::ScenePtr &scene,
        const geometry::Ray &initialRay,
        random::RandomizationHelper &randHelper
      );

      glm::vec3 pathRadiance(
        const RandomWalk &eyeWalk,
        const RandomWalk &lightWalk,
        int eyeLen,
        int lightLen
      );

    };
  }
}

#endif