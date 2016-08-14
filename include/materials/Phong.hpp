#ifndef PHONG_H_
#define PHONG_H_

#include "materials/AbstractMaterial.h"

namespace ray_storm
{
  namespace materials
  {
    class Phong : public AbstractMaterial
    {

    public:

      Phong(const glm::vec3 &kS, const glm::vec3 &Kd, float e, const glm::vec3 &emittance)
      {
        this->kS = kS;
        this->kD = kD;
        this->e = e;
        this->emittance = emittance;
      }

      glm::vec3 evaluateBRDF(const glm::vec3 &omegaIn, 
        const glm::vec3 &n, const glm::vec3 &omegaOut)
      {
        const glm::vec3 r = glm::normalize(glm::reflect(-omegaIn, n));
        return this->kD/static_cast<float>(M_PI) + (this->kS*std::pow(glm::dot(r, omegaOut), e))/glm::dot(n, omegaOut);
      }

      glm::vec3 getEmittance()
      {
        return this->emittance;
      }

      void drawReflectedRay(const glm::vec3 &omegaIn, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay)
      {
        const glm::vec3 r = glm::normalize(glm::reflect(-omegaIn, n));

        randRay.ray.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(r, this->e);
        randRay.ray.origin = position;
        randRay.inversePDF = randHelper.cosineRandomHemisphereInversePDF(dot(r, randRay.ray.direction), this->e);
      }

    private:

      glm::vec3 kS;

      glm::vec3 kD;

      float e;

      glm::vec3 emittance;

      
    };
  }
}

#endif