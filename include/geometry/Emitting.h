#ifndef EMITTING_H_
#define EMITTING_H_

#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace geometry
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

      virtual float getPDF() = 0;

      virtual glm::vec3 getEmittance()
      {
        return this->emittance;
      }

    private:

      glm::vec3 emittance;
    };
  }
}

#endif