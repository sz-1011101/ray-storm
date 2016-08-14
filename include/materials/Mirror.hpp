#ifndef MIRROR_H_
#define MIRROR_H_

#include "materials/AbstractMaterial.h"

namespace ray_storm
{
  namespace materials
  {
    class Mirror : public AbstractMaterial
    {
    public:

      Mirror(const glm::vec3 &reflectance)
      {
        this->reflectance = reflectance;
      }
      
      glm::vec3 evaluateBRDF(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v)
      {
        const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(-l, n));
        return glm::all(glm::equal(mirrorDirection, v)) ? this->reflectance : glm::vec3(0.0f);
      }

      glm::vec3 getEmittance()
      {
        // mirrors can't ever emit
        return glm::vec3(0.0f);
      }

      void drawReflectedRay(const glm::vec3 &in, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay)
      {
        const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(-in, n));

        randRay.ray.origin = position;
        randRay.ray.direction = mirrorDirection;
        randRay.inversePDF = 1.0f;
      }

    private:

      glm::vec3 reflectance;
      
    };
  }
}

#endif