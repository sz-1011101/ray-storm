#ifndef PHONG_H_
#define PHONG_H_

#include "materials/AbstractBRDF.h"

namespace ray_storm
{
  namespace materials
  {
    class Phong : public AbstractBRDF
    {

    public:

      Phong(const glm::vec3 &kD, const glm::vec3 &kS, float e)
      {
        // the famous three phong parameters
        this->kD = kD;
        this->kS = kS;
        this->e = e;

        this->lambertian = this->kD/static_cast<float>(M_PI);
        this->specular = this->kS*(this->e + 2.0f)/(2.0f*static_cast<float>(M_PI));
      }

      glm::vec3 evaluate(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v)
      {
        const glm::vec3 r = glm::normalize(glm::reflect(-l, n)); // ideal reflection of light
        return this->lambertian + this->specular*std::max(0.0f, std::pow(dot(r, v), this->e))/glm::dot(n, l);
      }

      void drawReflectedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
//#define UNIFORM_SAMPLING
#ifdef UNIFORM_SAMPLING
        randDir.direction = randHelper.drawUniformRandomHemisphereDirection(n);
        randDir.inversePDF = randHelper.uniformRandomHemisphereInversePDF();
#else 
        glm::vec3 r = glm::normalize(glm::reflect(in, n));
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(r, this->e);
        randDir.inversePDF = randHelper.cosineRandomHemisphereInversePDF(dot(r, randDir.direction), this->e);
#endif

      }

    private:

      glm::vec3 kS;
      glm::vec3 kD;
      float e;

      glm::vec3 lambertian;
      glm::vec3 specular;
      
    };
  }
}

#endif