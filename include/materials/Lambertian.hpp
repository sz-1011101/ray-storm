#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "utility/common.hpp"
#include "materials/AbstractBRDF.h"

namespace ray_storm
{
  namespace materials
  {
    
    class Lambertian : public AbstractBRDF
    {
    public:

      Lambertian(const glm::vec3 &albedo)
      {
        this->albedo = albedo; 
        this->constBrdf = this->albedo/static_cast<float>(M_PI);
      }

      glm::vec3 evaluate(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v)
      {
        // lambertian is constant!
        return this->constBrdf*std::max(0.0f, glm::dot(n, l));
      }

      void drawReflectedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(n, 1.0f);
        randDir.PDF = this->getPDF(in, n, randDir.direction);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &out
      )
      {
        return random::RandomizationHelper::cosineRandomHemispherePDF(dot(n, out), 1.0f);
      }

    private:

      glm::vec3 albedo;
      glm::vec3 constBrdf;
      
    };
  }
}

#endif