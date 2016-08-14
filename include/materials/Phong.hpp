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

      Phong(const glm::vec3 &kD, const glm::vec3 &kS, float e, const glm::vec3 &emittance)
      {
        // the famous three phong parameters
        this->kD = kD;
        this->kS = kS;
        this->e = e;

        this->lambertian = this->kD/static_cast<float>(M_PI);
        this->specular = this->kS*(this->e + 2.0f)/(2.0f*static_cast<float>(M_PI));
        
        this->emittance = emittance;
      }

      glm::vec3 evaluateBRDF(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v)
      {
        const glm::vec3 r = glm::normalize(glm::reflect(-l, n)); // ideal reflection of light
        return this->lambertian + this->specular*std::max(0.0f, std::pow(dot(r, v), this->e));
      }

      glm::vec3 getEmittance()
      {
        return this->emittance;
      }

      void drawReflectedRay(const glm::vec3 &in, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay)
      {
//#define UNIFORM_SAMPLING
#ifdef UNIFORM_SAMPLING
        randRay.ray.direction = randHelper.drawUniformRandomHemisphereDirection(n);
        randRay.ray.origin = position;
        randRay.inversePDF = randHelper.uniformRandomHemisphereInversePDF();
#else 
        glm::vec3 r = glm::normalize(glm::reflect(-in, n));
        randRay.ray.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(r, this->e);
        randRay.ray.origin = position;
        randRay.inversePDF = randHelper.cosineRandomHemisphereInversePDF(dot(r, randRay.ray.direction), this->e);
#endif

      }

    private:

      glm::vec3 kS;
      glm::vec3 kD;
      float e;

      glm::vec3 lambertian;
      glm::vec3 specular;

      glm::vec3 emittance;

      
    };
  }
}

#endif