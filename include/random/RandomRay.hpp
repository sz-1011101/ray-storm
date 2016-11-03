#ifndef RANDOM_RAY_H_
#define RANDOM_RAY_H_

#include "geometry/Ray.hpp"

namespace ray_storm
{
  namespace random
  {

    struct RandomRay
    {

      RandomRay() : delta(false) {};

      RandomRay(const geometry::Ray &ray, float PDF)
        : ray(ray), PDF(PDF), delta(false)
      {
      }

      RandomRay(const geometry::Ray &ray, bool delta) : ray(ray), PDF(0.0f), delta(delta)
      {
      }

      geometry::Ray ray;
      float PDF;
      bool delta;

    };


  }
}

#endif