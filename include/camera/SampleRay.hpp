#ifndef SAMPLE_RAY_H_
#define SAMPLE_RAY_H_

#include "geometry/Ray.hpp"

namespace ray_storm
{
  namespace camera
  {
    struct SampleRay
    {

      SampleRay()
      {
        
      }

      SampleRay(const geometry::Ray &ray, const glm::vec2 &xy) : ray(ray), xy(xy)
      {
        
      }

      geometry::Ray ray;
      glm::vec2 xy;
    };
  }
}

#endif