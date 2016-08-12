#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "utility/linear_alg.hpp"
#include "geometry/SimpleIntersection.hpp"

namespace ray_storm
{
  namespace geometry
  {

    template <typename T> struct Intersection
    {
      SimpleIntersection intersection;
      T *intersected;
      float t;

      Intersection() {
        this->intersected = nullptr;
      };

      Intersection(T *intersected, const SimpleIntersection &intersection, float t) {
        this->intersected = intersected;
        this->intersection = intersection;
        this->t = t;
      }

    };
  }
}

#endif