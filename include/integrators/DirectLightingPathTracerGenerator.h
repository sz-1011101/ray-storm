#ifndef DIRECT_LIGHTING_PATH_TRACER_GENERATOR_H_
#define DIRECT_LIGHTING_PATH_TRACER_GENERATOR_H_

#include "integrators/AbstractIntegratorGenerator.h"

namespace ray_storm
{
  namespace integrators
  {
    class DirectLightingPathTracerGenerator : public AbstractIntegratorGenerator
    {
    public:

      DirectLightingPathTracerGenerator(bool mis);

      AbstractIntegratorPtr createIntegrator();

    private:

      bool mis;
      
    };
  }
}

#endif