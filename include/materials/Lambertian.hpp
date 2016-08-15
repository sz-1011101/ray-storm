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
        return this->constBrdf;
      }

      void drawReflectedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
      //#define UNIFORM_SAMPLING
      #ifdef UNIFORM_SAMPLING // uniform sampling
        randDir.direction = randHelper.drawUniformRandomHemisphereDirection(n);
        randDir.inversePDF = randHelper.uniformRandomHemisphereInversePDF();
      #else // cosine weighted sampling!
        const float e = 1.0f;
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(n, e);
        randDir.inversePDF = randHelper.cosineRandomHemisphereInversePDF(dot(n, randDir.direction), e);
      #endif

      }

    private:

      glm::vec3 albedo;
      glm::vec3 constBrdf;
      
    };
  }
}

#endif