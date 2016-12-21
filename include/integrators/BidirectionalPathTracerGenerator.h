#ifndef BIDIRECTIONAL_PATH_TRACER_GENERATOR_H_
#define BIDIRECTIONAL_PATH_TRACER_GENERATOR_H_

#include "integrators/AbstractIntegratorGenerator.h"

namespace ray_storm
{
  namespace integrators
  {
    class BidirectionalPathTracerGenerator : public AbstractIntegratorGenerator
    {
    public:
      
      AbstractIntegratorPtr createIntegrator();
      
    };
  }
}

#endif