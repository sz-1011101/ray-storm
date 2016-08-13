#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint>
#include <cmath>
#include <stdio.h>

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
      
    };

    
  }
}

#endif