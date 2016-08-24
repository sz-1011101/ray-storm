#ifndef SCATTERING_GLASS_H_
#define SCATTERING_GLASS_H_

#include "materials/AbstractBTDF.h"

namespace ray_storm
{
  namespace materials
  {
    class ScatteringGlass : public AbstractBTDF
    {
    public:

      ScatteringGlass(
        const glm::vec3 &kD,
        const glm::vec3 &kS,
        float e,
        float indexOfRefraction = 1.5f
      ) : AbstractBTDF(indexOfRefraction)
      {
        this->kD = kD;
        this->kS = kS;
        this->e = e;
        this->samplingExponent = this->e;

        this->lambertian = this->kD/static_cast<float>(M_PI);
        this->specular = this->kS*(this->e + 2.0f)/(2.0f*static_cast<float>(M_PI));

        const float ll = glm::length(this->lambertian);
        const float ls = glm::length(this->specular);

        this->specProb = std::min(0.85f, ls/(ll + ls));
        this->diffProb = 1.0f - this->specProb;
      }

      glm::vec3 evaluate(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec3 &v
      )
      {
        glm::vec3 nRef = n;
        glm::vec3 t;
        MaterialHelper::refract(1.0f, this->indexOfRefraction, -l, n, t, nRef);

        return this->lambertian*std::abs(glm::dot(n, l)) + this->specular*glm::vec3(std::pow(std::max(0.0f, glm::dot(t, v)), this->e));
      }

      void drawRefractedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir
      )
      {
        const float prob = randHelper.drawUniformRandom();

        if (prob < this->specProb)
        {
          glm::vec3 nRef = n;
          glm::vec3 t;
          MaterialHelper::refract(1.0f, this->indexOfRefraction, in, n, t, nRef);
          randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(t, this->e);
        }
        else
        {
          const glm::vec3 nRef = glm::dot(in, n) < 0.0f ? -n : n;
          randDir.direction = randHelper.drawUniformRandomHemisphereDirection(nRef);
        }
        randDir.PDF = this->getPDF(in, n, randDir.direction);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &out
      )
      {
        glm::vec3 nRef = n;
        glm::vec3 t;
        MaterialHelper::refract(1.0f, this->indexOfRefraction, in, n, t, nRef);
        return this->specProb*random::RandomizationHelper::cosineRandomHemispherePDF(glm::dot(t, out), this->e)
          + this->diffProb*random::RandomizationHelper::uniformRandomHemispherePDF();
      }

    private:

      glm::vec3 kS;
      glm::vec3 kD;
      float e;

      glm::vec3 lambertian;
      glm::vec3 specular;

      float samplingExponent;
      float specProb;
      float diffProb;
      
    };
  }
}

#endif