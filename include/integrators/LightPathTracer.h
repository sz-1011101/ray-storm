#ifndef LIGHT_PATH_TRACER_H_
#define LIGHT_PATH_TRACER_H_

#include "integrators/AbstractIntegrator.h"
#include "integrators/RandomWalk.hpp"

namespace ray_storm
{
  namespace integrators
  {
    class LightPathTracer : public AbstractIntegrator
    {
    public:
      
      void sample(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      );

    private:

      RandomWalk walk;
      
    };
  }
}

#endif