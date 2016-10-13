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

      SimpleIntersection(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoords)
      {
        this->position = position;
        this->normal = normal;
        this->texCoords = texCoords;
      }

      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 texCoords;
    };
  }
}

#endif