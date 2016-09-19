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

    private:

      MaterialHelper();
      
    };
  }
}

#endif  