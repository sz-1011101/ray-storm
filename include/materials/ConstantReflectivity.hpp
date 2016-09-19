#ifndef CONSTANT_REFLECTIVITY_H_
#define CONSTANT_REFLECTIVITY_H_

#include "materials/AbstractReflectivity.hpp"

namespace ray_storm
{
  namespace materials
  {
    class ConstantReflectivity : public AbstractReflectivity
    {
    public:

      ConstantReflectivity(float reflectivity)
      {
        this->reflectivity = reflectivity;
      }

      virtual float computeF(float eta1, float eta2, const glm::vec3 &in, const glm::vec3 &n)
      {
        return this->reflectivity;
      }

    protected:

      float reflectivity;

    };
  }
}

#endif