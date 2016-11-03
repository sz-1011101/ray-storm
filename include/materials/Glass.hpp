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

      Glass(
        const glm::vec3 &color,
        float indexOfRefraction = 1.5f
      ) : AbstractBTDF(indexOfRefraction)
      {
        this->color = color;
      }

      glm::vec3 evaluate(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &v
      )
      {
        return this->color;
      }

      void drawDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir
      )
      {
        glm::vec3 nRef = n;
        MaterialHelper::refract(1.0f, this->indexOfRefraction, in, n, randDir.direction, nRef);
        randDir.PDF = this->getPDF(in, n, uv, randDir.direction);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &out
      )
      {
        return 1.0f;
      }

      bool delta() const
      {
        return true;
      }

    private:

      glm::vec3 color;
      
    };
  }
}

#endif