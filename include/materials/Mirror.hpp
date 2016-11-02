#ifndef MIRROR_H_
#define MIRROR_H_

#include "materials/AbstractBRDF.h"

namespace ray_storm
{
  namespace materials
  {
    class Mirror : public AbstractBRDF
    {
    public:

      Mirror(const glm::vec3 &reflectance)
      {
        this->reflectance = reflectance;
      }
      
      glm::vec3 evaluate(
        const glm::vec3 &l, 
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &v
      )
      {
        return glm::vec3(0.0f);
      }

      void drawDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(in, n));

        randDir.direction = mirrorDirection;
        randDir.PDF = this->getPDF(in, n, uv, mirrorDirection);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &out
      )
      {
        return 0.0f;
      }

      virtual bool delta() const
      {
        return true;
      }

    private:

      glm::vec3 reflectance;
      
    };
  }
}

#endif