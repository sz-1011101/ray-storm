#ifndef SCENE_H_
#define SCENE_H_

#include <memory>

#include "geometry/Object.h"
#include "geometry/Ray.hpp"
#include "datastructures/SpatialDatastructure.h"

namespace ray_storm
{
  namespace scene
  {
    
    class Scene
    {

    public:

      Scene();

      bool intersect(const geometry::Ray &ray, geometry::Intersection<geometry::Object> &intersection) const;

      void add(geometry::ObjectPtr &object);

      void finalize();

    private:

      // scene objects that interact with the light
      std::vector<geometry::ObjectPtr> objects;

      // for fast object intersecting
      std::unique_ptr<datastructures::SpatialDatastructure<geometry::Object>> dataStruct;
      
    };
  }
}

#endif