#ifndef GLASS_H_
#define GLASS_H_

#include "materials/AbstractBTDF.h"
#include "materials/MaterialHelper.hpp"

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
        const glm::vec3 &n, const glm::vec3 &v)
      {
        return this->color/std::abs(glm::dot(n, l));
      }

      bool drawRefractedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n, 
        float indexOfRefreaction,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        randDir.inversePDF = 1.0f;
        return MaterialHelper::refract(1.0f, indexOfRefreaction, in, n, randDir.direction);
      }

    private:

      glm::vec3 color;
      
    };
  }
}

#endif