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
        return this->reflectance/glm::dot(n, l);
      }

      void drawReflectedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {
        const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(in, n));

        randDir.direction = mirrorDirection;
        randDir.inversePDF = 1.0f;
      }

    private:

      glm::vec3 reflectance;
      
    };
  }
}

#endif