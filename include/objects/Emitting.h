#ifndef EMITTING_H_
#define EMITTING_H_

#include "random/RandomizationHelper.h"
#include "random/RandomRay.hpp"

namespace ray_storm
{
  namespace objects
  {
    class Emitting
    {
    public:

      Emitting(const glm::vec3 &emittance)
      {
        this->emittance = emittance;
      }

      virtual ~Emitting() {};

      virtual float getSurfaceArea() = 0;

      virtual glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper) = 0;

      virtual void drawRandomRay(random::RandomizationHelper &randHelper, random::RandomRay &randRay) = 0;

      virtual float getPDF() = 0;

      virtual glm::vec3 getEmittance()
      {
        return this->emittance;
      }

      virtual bool isEmitting()
      {
        if (glm::any(glm::greaterThan(this->emittance, glm::vec3(0.0f))))
        {
          return true;
        }

        return false;
      }

    private:

      glm::vec3 emittance;
    };
  }
}

#endif