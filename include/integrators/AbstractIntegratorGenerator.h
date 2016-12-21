#ifndef ABSTRACT_INTEGRATOR_GENERATOR_H_
#define ABSTRACT_INTEGRATOR_GENERATOR_H_

#include <memory>

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

    typedef std::shared_ptr<AbstractIntegratorGenerator> AbstractIntegratorGeneratorPtr;
  }
}

#endif