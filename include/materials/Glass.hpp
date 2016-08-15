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

      Glass(const glm::vec3 &color)
      {
        this->color = color;
      }

      glm::vec3 evaluate(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v, const glm::vec3 &r)
      {
        return this->color/std::abs(glm::dot(-n, l));
      }

      void drawRefractedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &r,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        randDir.inversePDF = 1.0f;
        randDir.direction = r;
      }

    private:

      glm::vec3 color;
      
    };
  }
}

#endif