#ifndef DIELECTRIC_FRESNEL_H_
#define DIELECTRIC_FRESNEL_H_

#include "materials/AbstractReflectivity.hpp"
#include "materials/MaterialHelper.hpp"

namespace ray_storm
{
  namespace materials
  {
    class DielectricFresnel : public AbstractReflectivity
    {
    public:

      virtual float computeF(float eta1, float eta2, const glm::vec3 &in, const glm::vec3 &n)
      {
        // see http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
        // and https://en.wikipedia.org/wiki/Fresnel_equations
        float _eta1 = eta1;
        float _eta2 = eta2;

        float cosIn = glm::dot(in, n);
        if (cosIn > 0.0f)
        {
          _eta1 = eta2;
          _eta2 = eta1;
        }

        float sinTr = _eta1/_eta2*std::sqrt(1.0f - cosIn*cosIn);

        if (sinTr >= 1.0f) // internal reflection
        {
          return 1.0f;
        }
        else // transmission possible
        {
          float cosTr = sqrt(std::max(0.0f, 1.0f - sinTr*sinTr));
          cosIn = std::abs(cosIn);
          // fresnel equations
          const float eta1CosIn = _eta1*cosIn;
          const float eta2CosIn = _eta2*cosIn;
          const float eta1CosTr = _eta1*cosTr;
          const float eta2CosTr = _eta2*cosTr;

          float Rs = (eta1CosIn - eta2CosTr)/(eta1CosIn + eta2CosTr);
          float Rp = (eta1CosTr - eta2CosIn)/(eta1CosTr + eta2CosIn);

          return (Rs*Rs + Rp*Rp)*0.5f; // average
        }
      }
      
    };
  }
}

#endif