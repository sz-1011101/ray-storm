#ifndef RAY_H_
#define RAY_H_

#include "utility/linear_alg.hpp"

namespace ray_storm
{
  namespace geometry
  {
    struct Ray
    {

      Ray() {};
      
      Ray(const glm::vec3 &origin, const glm::vec3 &direction) {
        this->origin = origin;
        this->direction = direction; 
      }

      glm::vec3 origin;
      glm::vec3 direction;
    };
  }
}

#endif