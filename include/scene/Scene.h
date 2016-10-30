#ifndef SCENE_H_
#define SCENE_H_

#include <memory>

#include "geometry/Emitter.h"
#include "geometry/Reflector.h"
#include "geometry/Ray.hpp"
#include "datastructures/SpatialDatastructure.h"
#include "random/RandomizationHelper.h"
#include "scene/AbstractSky.h"

namespace ray_storm
{
  namespace scene
  {
    
    class Scene
    {

    public:

      struct LuminaireSample
      {

        bool shadowed;

        glm::vec3 emittance;

        glm::vec3 direction;

        float PDF;

        LuminaireSample()
        {
          this->shadowed = true;
          this->emittance = glm::vec3(0.0f);
          this->PDF = 0.0f;
        }
      };

      Scene();

      bool intersect(const geometry::Ray &ray, geometry::Intersection<geometry::Object> &intersection) const;

      void sampleLuminaire(const glm::vec3 &x, const glm::vec3 &n, random::RandomizationHelper &randHelper, LuminaireSample &light);

      float getLuminairePDF(geometry::Object *object, const geometry::Ray &ray, const glm::vec3 &x, const glm::vec3 &n);

      void add(const geometry::EmitterPtr &emitter);

      void add(const geometry::ReflectorPtr &reflector);

      void finalize();

      void setSky(const AbstractSkyPtr &sky);

      glm::vec3 sampleSky(const glm::vec3 &direction);

      float getSkyPDF();

    private:

      // scene objects that interact with the light
      std::vector<geometry::ObjectPtr> objects;

      // scene objects that additionally can act as a light source
      std::vector<geometry::EmitterPtr> lights;

      // skylight
      AbstractSkyPtr sky;

      // for fast object intersecting
      std::unique_ptr<datastructures::SpatialDatastructure<geometry::Object>> dataStruct;
      
    };

    typedef std::shared_ptr<Scene> ScenePtr;
  }
}

#endif