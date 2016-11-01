#ifndef RAY_PACKAGE_H_
#define RAY_PACKAGE_H_

#include "utility/common.hpp"
#include "geometry/Ray.hpp"

#include <vector>

namespace ray_storm
{
  namespace camera
  {
    struct RayPackage
    {

      struct SampleRay
      {

        SampleRay() : weight(1.0f)
        {

        }

        SampleRay(const geometry::Ray &ray) : ray(ray), weight(1.0f)
        {
          
        }

        SampleRay(
          const geometry::Ray &ray,
          float weight
        ) : ray(ray), weight(weight) {};

        geometry::Ray ray;
        float weight;
      };

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