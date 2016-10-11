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

        SampleRay() : sample(0.0f), weight(0.0f)
        {

        }

        SampleRay(const geometry::Ray &ray) : ray(ray), sample(0.0f), weight(0.0f)
        {
          
        }

        SampleRay(
          const geometry::Ray &ray,
          const glm::vec3 &sample,
          float weight
        ) : ray(ray), sample(sample), weight(weight) {};

        geometry::Ray ray;
        glm::vec3 sample;
        float weight;
      };

      RayPackage(uint32_t samples) : rays(samples) {};

      SampleRay &operator[](std::size_t i)
      {
        return this->rays[i];
      }

      void setup(float x, float y)
      {
        this->x = x;
        this->y = y;
      }

      std::size_t size()
      {
        return this->rays.size();
      }

      glm::vec3 recombine()
      {
        glm::vec3 sum(0.0f);
        for (SampleRay &sr : this->rays)
        {
          sum += sr.weight*sr.sample;
        }
        return sum;
      }
      
      float x;
      float y;

      std::vector<SampleRay> rays;

    };
  }
}

#endif