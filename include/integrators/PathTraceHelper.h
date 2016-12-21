#ifndef PATH_TRACE_HELPER_H_
#define PATH_TRACE_HELPER_H_

#include "camera/AbstractCamera.h"
#include "scene/Scene.h"
#include "integrators/PathTraceVertex.hpp"

namespace ray_storm
{
  namespace integrators
  {
    class PathTraceHelper
    {
    private:

      PathTraceHelper();

    public:

      struct RandomWalk
      {
        RandomWalk() : absorbed(true) {}

        bool absorbed;
        std::vector<PathTraceVertex> vertices;
      };

      static void randomWalk(
        scene::Scene *scene,
        const geometry::Ray &initialRay,
        random::RandomizationHelper &randHelper,
        RandomWalk &walk,
        bool eye
      );

      // TODO move these out to seperate samplers?
      static void naive(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      static void directIllumination(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      static void directIlluminationBounce(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      static void lightPathTracing(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      static void bidirectional(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

      static glm::vec3 pathRadiance(
        const RandomWalk &eyeWalk,
        const RandomWalk &lightWalk,
        int eyeLen,
        int lightLen
      );

      static glm::vec3 pathDirectLighting(
        const RandomWalk &eyeWalk,
        scene::Scene *scene,
        random::RandomizationHelper &randHelper,
        bool weight
      );

      static glm::vec3 pathDirectLightingBounce(
        const RandomWalk &eyeWalk,
        scene::Scene *scene,
        random::RandomizationHelper &randHelper,
        bool weight
      );

      static glm::vec3 pathPathCombination(
        const glm::vec3 &Le,
        const RandomWalk &eyeWalk,
        const RandomWalk &lightWalk,
        scene::Scene *scene
      );

      static void pathLightPath(
        const glm::vec3 &Le,
        const RandomWalk &lightWalk,
        camera::AbstractCamera *camera,
        scene::Scene *scene,
        random::RandomizationHelper &randHelper,
        bool weight
      );

      static float pathWeighting(int eyeIndex, int lightIndex);
    };
  }
}

#endif