#ifndef SCENE_H_
#define SCENE_H_

#include <memory>

#include "geometry/Object.h"
#include "geometry/Ray.hpp"
#include "datastructures/SpatialDatastructure.h"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace scene
  {
    
    class Scene
    {

    public:

      struct LightSource
      {
        // object that is emitting
        geometry::Object *object;
        // emittance according to monte carlo estimation, don't use material emittance directly
        glm::vec3 emittance;
        // surface point that emitts
        glm::vec3 lightPos;

        LightSource()
        {
          this->object = nullptr;
          this->emittance = glm::vec3(0.0f);
        }
      };

      Scene();

      bool intersect(const geometry::Ray &ray, geometry::Intersection<geometry::Object> &intersection) const;

      bool drawRandomEmittingObject(random::RandomizationHelper &randHelper, LightSource &light);

      void add(geometry::ObjectPtr &object);

      void finalize();

      void setSky(const glm::vec3 &sky);

      const glm::vec3 &getSky();

    private:

      // scene objects that interact with the light
      std::vector<geometry::ObjectPtr> objects;

      // scene objects that additionally can act as a light source
      std::vector<geometry::ObjectPtr> lights;

      // for fast object intersecting
      std::unique_ptr<datastructures::SpatialDatastructure<geometry::Object>> dataStruct;

      glm::vec3 sky;
      
    };

    typedef std::shared_ptr<Scene> ScenePtr;
  }
}

#endif