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

      ScatteringGlass(const glm::vec3 &color, float e)
      {
        this->color = color;
        this->e = e;
        // Phong like specular transmission
        this->specular = this->color*(this->e + 2.0f)/(2.0f*static_cast<float>(M_PI));
      }

      glm::vec3 evaluate(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v, const glm::vec3 &r)
      {
        return this->specular*glm::vec3(std::pow(std::max(0.0f, glm::dot(r, v)), this->e));
      }

      void drawRefractedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &r, // ideal reflection of in
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
//#define UNIFORM_SAMPLING
#ifdef UNIFORM_SAMPLING
        const glm::vec3 nRef = glm::dot(n, in) < 0.0f ? -n : n;
        randDir.direction = randHelper.drawUniformRandomHemisphereDirection(nRef);
        randDir.inversePDF = randHelper.uniformRandomHemisphereInversePDF();
#else
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(r, this->e);
        randDir.inversePDF = randHelper.cosineRandomHemisphereInversePDF(dot(r, randDir.direction), this->e);
#endif
      }

    private:

      glm::vec3 color;

      float e;

      glm::vec3 specular;
      
    };
  }
}

#endif