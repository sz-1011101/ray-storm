#ifndef SPATIAL_DATASTRUCTURE_H_
#define SPATIAL_DATASTRUCTURE_H_

#include <vector>

#include "geometry/Intersection.hpp"

namespace ray_storm
{
  namespace datastructures
  {

    template<typename T> class SpatialDatastructure
    {

    public:

      virtual ~SpatialDatastructure() {};

      virtual void add(T *data) = 0;

      virtual void initialize() = 0;

      virtual bool intersect(const geometry::Ray &ray, geometry::Intersection<T> &intersection) = 0;

    };
  }
}

#endif