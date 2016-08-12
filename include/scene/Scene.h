#ifndef SCENE_H_
#define SCENE_H_

#include <memory>

#include "geometry/Object.h"
#include "datastructures/SpatialDatastructure.h"

namespace ray_storm
{
  namespace scene
  {
    
    class Scene
    {

    public:

      Scene();

      geometry::Object *intersect(const geometry::Ray &ray) const;

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