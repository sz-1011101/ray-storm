#ifndef INTEGRATOR_GENERATOR_FACTORY_H_
#define INTEGRATOR_GENERATOR_FACTORY_H_

#include "integrators/AbstractIntegratorGenerator.h"

namespace ray_storm
{
  namespace integrators
  {
    class IntegratorGeneratorFactory
    {
    private:

      IntegratorGeneratorFactory() {};

    public:

      AbstractIntegratorGeneratorPtr createPathTracerGenerator();

      AbstractIntegratorGeneratorPtr createDirectLightingPathTracerGenerator(bool mis);

      AbstractIntegratorGeneratorPtr createLightPathTracerGenerator();

      AbstractIntegratorGeneratorPtr createBidirectionalPathTracerGenerator();

    };
  }
}

#endif