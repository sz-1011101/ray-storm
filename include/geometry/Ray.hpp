#ifndef RAY_H_
#define RAY_H_

#include <iostream>

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

      friend std::ostream &operator<<(std::ostream &os, const Ray ray) {
        return os << ray.origin.x << "     " << ray.direction.x << '\n'
                  << ray.origin.y << " + t*" << ray.direction.y << '\n'
                  << ray.origin.z << "     " << ray.direction.z;
      }

      glm::vec3 origin;
      glm::vec3 direction;
    };
  }
}

#endif