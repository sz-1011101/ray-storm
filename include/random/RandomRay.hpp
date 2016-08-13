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

      RandomRay(const geometry::Ray &ray, float inversePDF)
      {
        this->ray = ray;
        this->inversePDF = inversePDF;
      }

      geometry::Ray ray;
      float inversePDF;

    };


  }
}

#endif