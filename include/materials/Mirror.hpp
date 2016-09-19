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
      
      glm::vec3 evaluate(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v)
      {
        const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(-l, n));
        return glm::dot(mirrorDirection, v) > 0.999f ? this->reflectance : glm::vec3(0.0f);
      }

      void drawDirection(
        const glm::vec3 &in,
        const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(in, n));

        randDir.direction = mirrorDirection;
        randDir.PDF = 1.0f;//this->getPDF(in, n, mirrorDirection);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &out
      )
      {
        const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(in, n));
        return glm::dot(mirrorDirection, out) > 0.999f ? 1.0f : 0.0f;
      }

    private:

      glm::vec3 reflectance;
      
    };
  }
}

#endif