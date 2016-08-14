#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "utility/common.hpp"
#include "materials/AbstractMaterial.h"
#include "random/RandomizationHelper.h"

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

      glm::vec3 evaluateBRDF(const glm::vec3 &omegaIn, 
        const glm::vec3 &n, const glm::vec3 &omegaOut)
      {
        // lambertian is constant!
        return this->constBrdf;
      }

      glm::vec3 getEmittance()
      {
        return this->emittance;
      }

      void drawReflectedRay(const glm::vec3 &omegaIn, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay)
      {

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

      glm::vec3 emittance;
      
    };
  }
}

#endif