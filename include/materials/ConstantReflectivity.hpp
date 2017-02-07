#ifndef CONSTANT_REFLECTIVITY_H_
#define CONSTANT_REFLECTIVITY_H_

#include "materials/AbstractReflectivity.h"

namespace ray_storm
{
  namespace materials
  {
    class ConstantReflectivity : public AbstractReflectivity
    {
    public:

      ConstantReflectivity(float reflectivity) : reflectivity(reflectivity)
      {
        
      }

      virtual ~ConstantReflectivity() {};

      virtual float computeF(float eta, const glm::vec3 &in, const glm::vec3 &n)
      {
        return this->reflectivity;
      }

    protected:

      float reflectivity;

    };
  }
}

#endif