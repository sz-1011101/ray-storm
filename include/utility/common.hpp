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
        return glm::vec3(r, M_PI + std::atan2(v.x, v.z), std::acos(v.y/r)); // radius, theta, phi
      }

      static glm::vec3 sphericalToCartesian(const glm::vec3 &v)
      {
        const float r = v.x;
        return glm::vec3(r*std::sin(v.y)*std::sin(v.z), r*std::cos(v.z), r*std::cos(v.y)*std::sin(v.z));
      }

      static glm::vec2 cartesianToSphericalToUV(const glm::vec3 &v)
      {
        glm::vec3 sv = cartesianToSpherical(v);
        return glm::vec2(sv.y/(2.0f*M_PI), sv.z/M_PI);
      }
      
    };

    
  }
}

#endif