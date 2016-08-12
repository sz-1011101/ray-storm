#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "utility/common.hpp"

namespace ray_storm
{
  namespace materials
  {
    
    class Lambertian : public AbstractMaterial
    {
    public:

      Lambertian(const glm::vec3 &albedo, const glm::vec3 &emittance)
      {
        this->albedo = albedo; 
        this->constBrdf = this->albedo/static_cast<float>(M_PI);
        this->emittance = emittance;
      }

      glm::vec3 evaluateBRDF(const glm::vec3 &v, const glm::vec3 &n, const glm::vec3 &l)
      {
        // lambertian is constant!
        return this->constBrdf;
      }

      glm::vec3 getEmittance() {
        return this->emittance;
      }
    
    private:

      glm::vec3 albedo;
      glm::vec3 constBrdf;

      glm::vec3 emittance;
      
    };
  }
}

#endif