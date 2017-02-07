#ifndef DIELECTRIC_FRESNEL_H_
#define DIELECTRIC_FRESNEL_H_

#include "materials/AbstractReflectivity.h"
#include "materials/MaterialHelper.hpp"

namespace ray_storm
{
  namespace materials
  {
    class DielectricFresnel : public AbstractReflectivity
    {
    public:

      DielectricFresnel(float indexOfRefraction) : indexOfRefraction(indexOfRefraction)
      {

      }

      virtual float computeF(float eta, const glm::vec3 &in, const glm::vec3 &n)
      {
        // see http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
        // and https://en.wikipedia.org/wiki/Fresnel_equations
        float eta1 = eta;
        float eta2 = this->indexOfRefraction;

        float cosIn = glm::dot(in, n);
        if (cosIn > 0.0f)
        {
          eta1 = eta2;
          eta2 = eta1;
        }

        float sinTr = eta1/eta2*std::sqrt(1.0f - cosIn*cosIn);

        if (sinTr >= 1.0f) // internal reflection
        {
          return 1.0f;
        }
        else // transmission possible
        {
          float cosTr = sqrt(std::max(0.0f, 1.0f - sinTr*sinTr));
          cosIn = std::abs(cosIn);
          // fresnel equations
          const float eta1CosIn = eta1*cosIn;
          const float eta2CosIn = eta2*cosIn;
          const float eta1CosTr = eta1*cosTr;
          const float eta2CosTr = eta2*cosTr;

          float Rs = (eta1CosIn - eta2CosTr)/(eta1CosIn + eta2CosTr);
          float Rp = (eta1CosTr - eta2CosIn)/(eta1CosTr + eta2CosIn);

          return (Rs*Rs + Rp*Rp)*0.5f; // average
        }
      }

    private:

      float indexOfRefraction;
      
    };
  }
}

#endif