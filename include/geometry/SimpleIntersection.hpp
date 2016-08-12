#ifndef SIMPLE_INTERSECTION_H_
#define SIMPLE_INTERSECTION_H_

#include "utility/linear_alg.hpp"

namespace ray_storm
{
  namespace geometry
  {
    struct SimpleIntersection
    {

      SimpleIntersection() {};

      SimpleIntersection(const glm::vec3 &position, const glm::vec3 &normal)
      {
        this->position = position;
        this->normal = normal;
      }

      glm::vec3 position;
      glm::vec3 normal;
    };
  }
}

#endif