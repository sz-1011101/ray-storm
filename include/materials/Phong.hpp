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

      glm::vec3 evaluate(
        const glm::vec3 &l, 
        const glm::vec3 &n,
        const glm::vec3 &v
      )
      {
        const glm::vec3 r = glm::normalize(glm::reflect(-l, n)); // ideal reflection of light
        return this->lambertian*std::max(0.0f, glm::dot(n, l)) + 
          this->specular*std::pow(std::max(0.0f, dot(r, v)), this->e);
      }

      void drawReflectedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {

        glm::vec3 r = glm::normalize(glm::reflect(in, n));
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(r, this->e);
        randDir.PDF = this->getPDF(in, n, randDir.direction);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &out
      )
      {
        glm::vec3 r = glm::normalize(glm::reflect(in, n));
        return random::RandomizationHelper::cosineRandomHemispherePDF(dot(r, out), this->e);
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