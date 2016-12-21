#ifndef ABSTRACT_INTEGRATOR_GENERATOR_H_
#define ABSTRACT_INTEGRATOR_GENERATOR_H_

#include "integrators/AbstractIntegrator.h"

namespace ray_storm
{
  namespace integrators
  {
    class AbstractIntegratorGenerator
    {
    public:

      virtual ~AbstractIntegratorGenerator() {};

      virtual AbstractIntegratorPtr createIntegrator() = 0;
      
    };
  }
}

#endif