#ifndef INTEGRATOR_FACTORY_H_
#define INTEGRATOR_FACTORY_H_

#include "integrators/AbstractIntegrator.h"

namespace ray_storm
{
  namespace integrators
  {

    class IntegratorFactory
    {
    private:

      IntegratorFactory() {};

    public:

      static AbstractIntegratorPtr createPathTracer();

      static AbstractIntegratorPtr createDirectLightingPathTracer(bool mis);

      static AbstractIntegratorPtr createLightPathTracer();

      static AbstractIntegratorPtr createBidirectionalPathTracer();

    };
  }
}

#endif