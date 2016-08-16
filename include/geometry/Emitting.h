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

      virtual ~Emitting() {};

      virtual float getSurfaceArea() = 0;

      virtual glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper) = 0;
    };
  }
}

#endif