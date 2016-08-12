
#ifndef INTERSECTABLE_H_
#define INTERSECTABLE_H_

#include "geometry/Ray.hpp"
#include "geometry/Intersection.hpp"

namespace ray_storm
{
  namespace geometry
  {
    template <typename T> class Intersectable
    {

    public:

      virtual ~Intersectable() {};

      virtual bool intersect(const Ray &ray, Intersection<T> &intersection) = 0;
      
    };
  }
}

#endif