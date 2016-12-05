#ifndef RAY_MARCHER_H_
#define RAY_MARCHER_H_

#include "geometry/Marchable.h"
#include "geometry/SimpleIntersection.hpp"
#include "geometry/Ray.hpp"

namespace ray_storm
{
  namespace geometry
  {

    class RayMarcher
    {
    public:

      static bool march(const Ray &ray, Marchable *marchable, SimpleIntersection &intersection)
      {
        const float eps = 0.001f;
        const int itr = 80;
        const glm::vec3 &center = marchable->getCenter();
        // go to object space
        glm::vec3 current = ray.origin - center;
        float stepLen = 0.0f;
        for (int i = 0; i < itr; i++)
        {
          stepLen = std::abs(marchable->distance(current));
          if (stepLen < eps)
          {
            intersection.normal = marchable->estimateNormal(current);
            intersection.position = current + center;
            return true;
          }
          current += stepLen*ray.direction;
        }

        return false;
      }

    private:

      RayMarcher() {};

    };

  }
}

#endif