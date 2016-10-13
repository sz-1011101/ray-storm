#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint>
#include <cmath>
#include <stdio.h>

#include "utility/linear_alg.hpp"

namespace ray_storm
{
  namespace utility
  {

    class Math
    {

    public:

      static float degToRad(float degree)
      {
        return (degree/180.0f)*M_PI;
      }

      static float radToDeg(float radians)
      {
        return radians/M_PI * 180.0f;
      }

      static glm::vec3 cartesianToSpherical(const glm::vec3 &v)
      {
        const float r = glm::length(v);
        // http://mathworld.wolfram.com/SphericalCoordinates.html, but different axes
        return glm::vec3(r, std::atan(v.x/v.z), std::acos(v.y/r));
      }
      
    };

    
  }
}

#endif