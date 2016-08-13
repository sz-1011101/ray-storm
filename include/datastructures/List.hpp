#ifndef LIST_H_
#define LIST_H_

#include "datastructures/SpatialDatastructure.h"

namespace ray_storm
{
  namespace datastructures
  {
    template<typename T> class List : public SpatialDatastructure<T>
    {

    public:

      void add(T *data)
      {
        this->data.push_back(data);
      }

      void initialize()
      {
        // not necessary
      }

      bool intersect(const geometry::Ray &ray, geometry::Intersection<T> &intersection)
      {
        geometry::Intersection<T> currentClosest;
        bool objHit = false;

        for (T* d : this->data) {
          
          geometry::Intersection<T> currIntersect;

          // we hit the object?
          if (!d->intersect(ray, currIntersect)) {
            continue;
          }

          // first object ever found!
          if (!objHit) {
            currentClosest = currIntersect;
            objHit = true;
            continue;
          }

          // current intersection is actually closer?
          if (currIntersect.t < currentClosest.t) {
            currentClosest = currIntersect;
          }
        }

        return objHit;
      }

    private:

      std::vector<T*> data;
      
    };
  }
}

#endif