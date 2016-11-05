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

      void sample(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
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
        scene::Scene *scene,
        const geometry::Ray &initialRay,
        random::RandomizationHelper &randHelper,
        RandomWalk &walk
      );

      // TODO move these out to seperate samplers?
      void naive(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      void directIllumination(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      void directIlluminationBounce(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      void bidirectional(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      glm::vec3 pathRadiance(
        const RandomWalk &eyeWalk,
        const RandomWalk &lightWalk,
        int eyeLen,
        int lightLen
      );

      glm::vec3 pathDirectLighting(
        const RandomWalk &eyeWalk,
        scene::Scene *scene,
        random::RandomizationHelper &randHelper
      );

      glm::vec3 pathPathCombination(
        const glm::vec3 &Le,
        const RandomWalk &eyeWalk,
        const RandomWalk &lightWalk,
        scene::Scene *scene
      );

      float pathWeighting(int eyeIndex, int lightIndex);

    };
  }
}

#endif