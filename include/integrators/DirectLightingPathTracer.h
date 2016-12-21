#ifndef DIRECT_LIGHTING_PATH_TRACER_H_
#define DIRECT_LIGHTING_PATH_TRACER_H_

#include "integrators/AbstractIntegrator.h"
#include "integrators/RandomWalk.hpp"

namespace ray_storm
{
  namespace integrators
  {
    class DirectLightingPathTracer : public AbstractIntegrator
    {
    public:

      DirectLightingPathTracer(bool mis);

      void sample(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

    private:

      bool mis;
      
      RandomWalk walk;
      
    };
  }
}

#endif