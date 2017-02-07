#ifndef CONDUCTOR_FRESNEL_H_
#define CONDUCTOR_FRESNEL_H_

#include "materials/AbstractReflectivity.h"

namespace ray_storm
{
  namespace materials
  {
    class ConductorFresnel : public AbstractReflectivity
    {
    public:

      ConductorFresnel(
        float indexOfRefraction,
        float absorption
      ) : indexOfRefraction(indexOfRefraction), absorption(absorption)
      {

      }

      float computeF(float eta, const glm::vec3 &in, const glm::vec3 &n)
      {
        const float eta2 = this->indexOfRefraction;
        float cosIn = glm::dot(-in, n);
        const float t1 = eta2*eta2 + this->absorption*this->absorption;
        const float cosIn2 = cosIn*cosIn;
        const float t2 = t1*cosIn2;

        const float _2eta_cosIn = 2.0f*eta2*cosIn;
        const float Rs2 = (t2 - _2eta_cosIn + 1.0f)/(t2 + _2eta_cosIn + 1.0f);

        const float Rp2 = (t1 - _2eta_cosIn + cosIn2)/(t1 + _2eta_cosIn + cosIn2);

        return (Rs2 + Rp2)*0.5f;
      }
    
    private:

      float indexOfRefraction;

      float absorption;

    };
  }
}

#endif