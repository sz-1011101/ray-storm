#ifndef PATH_TRACER_GENERATOR_H_
#define PATH_TRACER_GENERATOR_H_

#include "integrators/AbstractIntegratorGenerator.h"

namespace ray_storm
{
  namespace integrators
  {
    class PathTracerGenerator : public AbstractIntegratorGenerator
    {
    public:

      AbstractIntegratorPtr createIntegrator();
      
    };
  }
}

#endif