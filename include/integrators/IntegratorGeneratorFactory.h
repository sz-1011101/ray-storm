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

      static AbstractIntegratorGeneratorPtr createPathTracerGenerator();

      static AbstractIntegratorGeneratorPtr createDirectLightingPathTracerGenerator(bool mis);

      static AbstractIntegratorGeneratorPtr createLightPathTracerGenerator();

      static AbstractIntegratorGeneratorPtr createBidirectionalPathTracerGenerator();

    };
  }
}

#endif