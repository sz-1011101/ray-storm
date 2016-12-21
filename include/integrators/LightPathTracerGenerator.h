#ifndef LIGHT_PATH_TRACER_GENERATOR_H_
#define LIGHT_PATH_TRACER_GENERATOR_H_

#include "integrators/AbstractIntegratorGenerator.h"

namespace ray_storm
{
  namespace integrators
  {
    class LightPathTracerGenerator : public AbstractIntegratorGenerator
    {
    public:

      AbstractIntegratorPtr createIntegrator();
      
    };
  }
}

#endif