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

      ScatteringGlass(const glm::vec3 &color, float e, float indexOfRefraction = 1.35f)
      {
        this->color = color;
        this->e = e;
        this->indexOfRefraction = indexOfRefraction;
        // Phong like specular transmission
        this->specular = this->color*(this->e + 2.0f)/(2.0f*static_cast<float>(M_PI));
      }

      glm::vec3 evaluate(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec3 &v
      )
      {
        glm::vec3 t;
        MaterialHelper::refract(this->indexOfRefraction, 1.0f, -l, -n, t);

        return this->specular*glm::vec3(std::pow(std::max(0.0f, glm::dot(t, v)), this->e));
      }

      bool drawRefractedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
//#define UNIFORM_SAMPLING
#ifdef UNIFORM_SAMPLING
        const glm::vec3 nRef = glm::dot(n, in) < 0.0f ? -n : n;
        randDir.direction = randHelper.drawUniformRandomHemisphereDirection(nRef);
        randDir.inversePDF = randHelper.uniformRandomHemisphereInversePDF();
#else
        glm::vec3 t;
        const bool refract = MaterialHelper::refract(1.0f, this->indexOfRefraction, in, n, t);
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(t, this->e);
        randDir.inversePDF = randHelper.cosineRandomHemisphereInversePDF(dot(t, randDir.direction), this->e);

        return refract;
#endif
      }

    private:

      glm::vec3 color;

      float e;

      float indexOfRefraction;

      glm::vec3 specular;
      
    };
  }
}

#endif