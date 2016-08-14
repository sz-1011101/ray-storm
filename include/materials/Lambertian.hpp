#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "utility/common.hpp"
#include "materials/AbstractBRDF.h"
#include "random/RandomizationHelper.h"

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

      void drawReflectedRay(const glm::vec3 &in, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay)
      {
      //#define UNIFORM_SAMPLING
      #ifdef UNIFORM_SAMPLING // uniform sampling
        randRay.ray.direction = randHelper.drawUniformRandomHemisphereDirection(n);
        randRay.ray.origin = position;
        randRay.inversePDF = randHelper.uniformRandomHemisphereInversePDF();
      #else // cosine weighted sampling!
        const float e = 1.0f;
        randRay.ray.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(n, e);
        randRay.ray.origin = position;
        randRay.inversePDF = randHelper.cosineRandomHemisphereInversePDF(dot(n, randRay.ray.direction), e);
      #endif

      }

    private:

      glm::vec3 albedo;
      glm::vec3 constBrdf;
      
    };
  }
}

#endif