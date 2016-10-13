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
        const glm::vec2 &uv,
        const glm::vec3 &v
      )
      {
        float cosTerm = glm::dot(n, l);
        if (cosTerm < 0.0f)
        {
          return glm::vec3(0.0f);
        }
        const glm::vec3 r = glm::normalize(glm::reflect(-l, n)); // ideal reflection of light
        return this->lambertian*cosTerm + 
          this->specular*std::pow(std::max(0.0f, dot(r, v)), this->e);
      }

      void drawDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        // MIS the phong brdf
        const float prob = randHelper.drawUniformRandom();

        if (prob < this->specProb)
        {
          glm::vec3 r = glm::normalize(glm::reflect(in, n));
          randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(r, this->samplingExponent);
        }
        else
        {
          randDir.direction = randHelper.drawUniformRandomHemisphereDirection(n);
        }

        randDir.PDF = this->getPDF(in, n, uv, randDir.direction);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &out
      )
      {
        glm::vec3 r = glm::normalize(glm::reflect(in, n));

        // specular pdf + diffuse pdf
        return this->specProb*random::RandomizationHelper::cosineRandomHemispherePDF(glm::dot(r, out), this->samplingExponent) +
          this->diffProb*random::RandomizationHelper::uniformRandomHemispherePDF();
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