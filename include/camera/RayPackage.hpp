#ifndef RAY_PACKAGE_H_
#define RAY_PACKAGE_H_

#include "utility/common.hpp"
#include "geometry/Ray.hpp"
#include "camera/SampleRay.hpp"

#include <vector>

namespace ray_storm
{
  namespace camera
  {
    struct RayPackage
    {

      RayPackage(uint32_t samples) : rays(samples) {};

      SampleRay &operator[](std::size_t i)
      {
        return this->rays[i];
      }

      void setup(const glm::vec2 &xy)
      {
        this->xy = xy;
      }

      std::size_t size()
      {
        return this->rays.size();
      }
      
      glm::vec2 xy;

      std::vector<SampleRay> rays;

    };
  }
}

#endif