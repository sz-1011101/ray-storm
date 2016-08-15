#ifndef MATERIAL_HELPER_H_
#define MATERIAL_HELPER_H_

namespace ray_storm
{
  namespace materials
  {
    class MaterialHelper
    {
    public:

      static bool refract(float eta1, float eta2, const glm::vec3 &in, const glm::vec3 &n, glm::vec3 &r)
      {
        // see http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
        glm::vec3 nRef = n;
        float _eta1 = eta1;
        float _eta2 = eta2;
        
        float cosThetaIn = glm::dot(nRef, in);

        if (cosThetaIn < 0.0f) // outside, entering material
        {
          cosThetaIn = -cosThetaIn;
        }
        else // inside, leaving the material to freedom
        {
          nRef = -n;
          _eta1 = eta2;
          _eta2 = eta1;
        }

        const float etaRatio = _eta1/_eta2;
        const float c1 = cosThetaIn;
        const float innerC2 = 1.0f - etaRatio*etaRatio*(1.0f - cosThetaIn*cosThetaIn);

        // total inner reflection
        if (innerC2 < 0.0f)
        {
          return false;
        }
        const float c2 = std::sqrt(innerC2);

        r =  glm::normalize(etaRatio*in + (etaRatio*c1 - c2)*nRef);
        return true;
      }

    private:

      MaterialHelper();
      
    };
  }
}

#endif  