#ifndef RAY_MARCHER_H_
#define RAY_MARCHER_H_

#include "geometry/Marchable.h"
#include "geometry/SimpleIntersection.hpp"
#include "geometry/Ray.hpp"
#include "geometry/AxisAlignedBox.hpp"

namespace ray_storm
{
  namespace geometry
  {

    class RayMarcher
    {
    public:

      static bool march(
        const Ray &ray,
        Marchable *marchable,
        SimpleIntersection &intersection,
        uint32_t itr = 80
      )
      {
        const float eps = 0.001f;

        glm::vec3 current = ray.origin;
        float stepLen = 0.0f;
        for (uint32_t i = 0; i < itr; i++)
        {
          stepLen = std::abs(marchable->distance(current));
          if (stepLen < eps)
          {
            intersection.normal = marchable->estimateNormal(current);
            intersection.position = current;
            return true;
          }
          current += stepLen*ray.direction;
        }

        return false;
      }

      static bool march(
        const Ray &ray,
        const AxisAlignedBox::BBoxIntersectionInterval &interval,
        Marchable *marchable,
        SimpleIntersection &intersection,
        uint32_t itr = 80
      )
      {
        // ray starts behind bbox...
        if (interval.tmin < 0.0f && interval.tmax < 0.0f)
        {
          return false;
        }

        const float eps = 0.001f;
        
        glm::vec3 current;
        float maxLen;
        // ray outside of bbox, going in?
        if (interval.tmin > interval.tmax)
        {
          current = ray.origin + interval.tmin*ray.direction;
          maxLen = interval.tmax - interval.tmin;
        }
        else // ray inside bbox
        {
          current = ray.origin;
          maxLen = interval.tmax;
        }

        float walked = 0.0f;
        for (uint32_t i = 0; i < itr; i++)
        {
          // we passed the bbox bound?
          if (walked > maxLen)
          {
            return false;
          }
          const float stepLen = std::abs(marchable->distance(current));
          if (stepLen < eps)
          {
            // TODO set uv
            intersection.normal = marchable->estimateNormal(current);
            intersection.position = current;
            intersection.texCoords = marchable->computeTexCoords(current);
            return true;
          }
          // do a step
          current += stepLen*ray.direction;
          walked += stepLen;
        }

        return false;
      }

    private:

      RayMarcher() {};

    };

  }
}

#endif