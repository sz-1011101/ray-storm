#ifndef ABSTRACT_INTEGRATOR_H_
#define ABSTRACT_INTEGRATOR_H_

#include <memory>

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

  namespace integrators
  {
    class AbstractIntegrator
    {
    public:
      
      virtual ~AbstractIntegrator() {};

      virtual void sample(
        scene::Scene *scene,
        camera::AbstractCamera *camera,
        const camera::SampleRay &sampleRay,
        random::RandomizationHelper &randHelper
      ) = 0;

    };

    typedef std::shared_ptr<AbstractIntegrator> AbstractIntegratorPtr;
  }
}

#endif