#ifndef PATH_TRACE_VERTEX_H_
#define PATH_TRACE_VERTEX_H_

#include "geometry/Ray.hpp"
#include "scene/Scene.h"
#include "random/RandomizationHelper.h"
#include "geometry/Object.h"

namespace ray_storm
{
  namespace renderer
  {
    class PathTraceVertex
    {
    public:

      PathTraceVertex() : object(nullptr), material(nullptr) {}

      PathTraceVertex(const geometry::Ray &incoming) : incoming(incoming), object(nullptr), material(nullptr) {}

      void setupNext(PathTraceVertex &vertex)
      {
        vertex = PathTraceVertex(this->bounceRay.ray);
      }

      bool computeIntersection(scene::Scene *scene)
      {
        geometry::Intersection<geometry::Object> isect;
        if (!scene->intersect(this->incoming, isect))
        {
          return false;
        }

        this->position = isect.intersection.position;
        this->normal = isect.intersection.normal;
        this->uv = isect.intersection.texCoords;
        this->object = isect.intersected;
        this->material = this->object->getMaterial();
        return true;
      }

      bool computeBounce(random::RandomizationHelper &randHelper)
      {
        if (this->material == nullptr ||
          !this->material->sampleBSDF(this->incoming.direction, this->position, this->normal, this->uv, randHelper, this->bounceRay))
        {
          return false;
        }

        return true;
      }

      const geometry::Ray &getBounceRay()
      {
        return this->bounceRay.ray;
      }

      const geometry::Ray &getIncoming()
      {
        return this->incoming;
      }

      bool isIntersecting()
      {
        return object != nullptr;
      }

      const glm::vec3 &getPosition()
      {
        return this->position;
      }

      const glm::vec3 &getNormal()
      {
        return this->normal;
      }

      bool computeBounceIncomingBSDF(glm::vec3 &bsdf)
      {
        if (this->material == nullptr) {
          return false;
        }
        bsdf = this->material->evaluateBSDF(this->bounceRay.ray.direction, this->normal, this->uv, -this->incoming.direction);
        return true;
      }

      float getBounceIncomingPDF()
      {
        return this->bounceRay.PDF;
      }

      bool computeEmittance(glm::vec3 &emittance)
      {
        if (this->object == nullptr)
        {
          return false;
        }
        dispatchers::EmittanceDispatcher ed;
        this->object->accept(&ed);
        emittance = ed.getEmittance();
        return true;
      }

    private:

      // vertex situation

      geometry::Ray incoming;

      glm::vec3 position;

      glm::vec3 normal;

      glm::vec2 uv;

      random::RandomRay bounceRay;

      // additional info

      geometry::Object *object;

      materials::Material *material;
      
    };
  }
}

#endif