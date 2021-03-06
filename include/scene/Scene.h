#ifndef SCENE_H_
#define SCENE_H_

#include <memory>

#include "geometry/Bounded.hpp"
#include "geometry/SpherePrimitive.hpp"
#include "objects/Emitter.h"
#include "objects/Reflector.h"
#include "geometry/Ray.hpp"
#include "datastructures/SpatialDatastructure.h"
#include "random/RandomizationHelper.h"
#include "random/RandomRay.hpp"
#include "scene/AbstractSky.h"

namespace ray_storm
{
  namespace scene
  {
    
    class Scene : public geometry::Bounded
    {

    public:

      struct LuminaireSample
      {

        bool shadowed;

        glm::vec3 emittance;

        glm::vec3 direction;

        float PDF;

        bool delta;

        LuminaireSample()
        {
          this->shadowed = true;
          this->emittance = glm::vec3(0.0f);
          this->PDF = 0.0f;
          this->delta = false;
        }
      };

      struct LuminaireRay
      {
        glm::vec3 emittance;

        random::RandomRay randRay;

        bool directional;

        LuminaireRay()
        {
          this->emittance = glm::vec3(0.0f);
          this->randRay.PDF = 0.0f;
          this->directional = false;
        }
      };

      Scene();

      bool intersect(const geometry::Ray &ray, geometry::Intersection<objects::Object> &intersection) const;

      void sampleLuminaire(const glm::vec3 &x, const glm::vec3 &n, random::RandomizationHelper &randHelper, LuminaireSample &light);

      bool sampleLuminaireRay(random::RandomizationHelper &randHelper, LuminaireRay &lumRay);

      float getLuminairePDF(
        objects::Object *object,
        const geometry::Ray &ray,
        const glm::vec3 &x,
        const glm::vec3 &n,
        const glm::vec2 &uv
      );

      void add(const objects::EmitterPtr &emitter);

      void add(const objects::ReflectorPtr &reflector);

      void finalize();

      void setSky(const AbstractSkyPtr &sky);

      glm::vec3 sampleSky(const glm::vec3 &direction);

      float getSkyPDF();

      bool visible(const glm::vec3 &origin, const glm::vec3 &target);

      bool visible(const glm::vec3 &origin, const glm::vec3 &target, const glm::vec3 &targetNormal);

    private:

      // scene objects that interact with the light
      std::vector<objects::ObjectPtr> objects;

      // scene objects that additionally can act as a light source
      std::vector<objects::EmitterPtr> lights;

      // skylight
      AbstractSkyPtr sky;

      // for fast object intersecting
      std::unique_ptr<datastructures::SpatialDatastructure<objects::Object>> dataStruct;

      geometry::SpherePrimitive boundingSphere;
      
    };

    typedef std::shared_ptr<Scene> ScenePtr;
  }
}

#endif