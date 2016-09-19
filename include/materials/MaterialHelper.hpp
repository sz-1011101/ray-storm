#ifndef MATERIAL_HELPER_H_
#define MATERIAL_HELPER_H_

#include "utility/linear_alg.hpp"
#include "materials/LIGHT_INTERACTION_TYPE.h"

namespace ray_storm
{
  namespace materials
  {
    class MaterialHelper
    {
    public:

      static LIGHT_INTERACTION_TYPE determineType(const glm::vec3 &l, const glm::vec3 &n, const glm::vec3 &v)
      {
        float cosL = dot(n, l);
        float cosV = dot(n, v);
        // we have a reflection if both cosines are greater zero or both are smaller zero (reflection at the anti normal)
        return ((cosL >= 0 && cosV >= 0) || (cosL <= 0 && cosV <= 0)) ? REFLECTION : REFRACTION;
      }

      static void correctRefractSituation(const glm::vec3 &in, float &eta1, float &eta2, glm::vec3 &n)
      {
        if (glm::dot(n, in) > 0.0f)
        {
          float temp = eta1;
          eta1 = eta2;
          eta2 = temp;
          n = -n;
        }

      }

      static bool refract(
        float eta1,
        float eta2,
        const glm::vec3 &in,
        const glm::vec3 &n,
        glm::vec3 &t,
        glm::vec3 &nRef
      )
      {
        // see http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
        nRef = n;
        float _eta1 = eta1;
        float _eta2 = eta2;
        MaterialHelper::correctRefractSituation(in, _eta1, _eta2, nRef);
        float cosThetaIn = -glm::dot(nRef, in);

        const float etaRatio = _eta1/_eta2;
        const float c1 = cosThetaIn;
        const float innerC2 = 1.0f - etaRatio*etaRatio*(1.0f - cosThetaIn*cosThetaIn);

        // total inner reflection
        if (innerC2 < 0.0f)
        {
          t = glm::reflect(in, nRef);
          return false;
        }
        const float c2 = std::sqrt(innerC2);

        t =  glm::normalize(etaRatio*in + (etaRatio*c1 - c2)*nRef);
        return true;
      }

      static float computeFresnelReflection(float eta1, float eta2, const glm::vec3 &in, const glm::vec3 &n)
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

      static float computeFresnelReflection(float ior, const glm::vec3 &in, const glm::vec3 &n)
      {
        const glm::vec3 nRef = glm::dot(n, in) < 0.0f ? n : -n;
        const glm::vec3 r = glm::reflect(in, nRef);
        const glm::vec3 v = -in;
        const glm::vec3 h = (r + v)/glm::length(r + v);

        const float c = glm::dot(h, v);
        const float g = ior*ior + c*c - 1.0f;

        const float g_minus_c = g - c;
        const float g_plus_c = g + c;

        // first term
        const float t1 = (g_minus_c*g_minus_c)/(2.0f*g_plus_c*g_plus_c);

        // second term
        const float td = (c*g_plus_c - 1.0f)/(c*(g_minus_c) + 1.0f);
        const float t2 = 1.0f + td*td;

        return t1*t2;
      }

    private:

      MaterialHelper();
      
    };
  }
}

#endif  