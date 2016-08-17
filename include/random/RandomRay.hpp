#ifndef RANDOM_RAY_H_
#define RANDOM_RAY_H_

#include "geometry/Ray.hpp"

namespace ray_storm
{
  namespace random
  {

    struct RandomRay
    {

      RandomRay() {};

      RandomRay(const geometry::Ray &ray, float PDF)
      {
        this->ray = ray;
        this->PDF = PDF;
      }

      geometry::Ray ray;
      float PDF;

    };


  }
}

#endif