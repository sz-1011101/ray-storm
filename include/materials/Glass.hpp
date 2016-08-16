#ifndef GLASS_H_
#define GLASS_H_

#include "materials/AbstractBTDF.h"

namespace ray_storm
{
  namespace materials
  {
    class Glass : public AbstractBTDF
    {
    public:

      Glass(const glm::vec3 &color, float indexOfRefraction = 1.5f)
      {
        this->color = color;
        this->indexOfRefraction = indexOfRefraction;
      }

      glm::vec3 evaluate(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec3 &v
      )
      {
        return this->color;
      }

      void drawRefractedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        MaterialHelper::refract(1.0f, this->indexOfRefraction, in, n, randDir.direction);
        randDir.inversePDF = 1.0f;
      }

    private:

      glm::vec3 color;

      float indexOfRefraction;
      
    };
  }
}

#endif