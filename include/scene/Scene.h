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

      void setSky(const glm::vec3 &sky);

      const glm::vec3 &getSky();

    private:

      // scene objects that interact with the light
      std::vector<geometry::ObjectPtr> objects;

      // for fast object intersecting
      std::unique_ptr<datastructures::SpatialDatastructure<geometry::Object>> dataStruct;

      glm::vec3 sky;
      
    };

    typedef std::shared_ptr<Scene> ScenePtr;
  }
}

#endif