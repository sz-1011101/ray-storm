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
        return this->object != nullptr;
      }

      bool isReflecting()
      {
        return this->material != nullptr;
      }

      const glm::vec3 &getPosition()
      {
        return this->position;
      }

      const glm::vec3 &getNormal()
      {
        return this->normal;
      }

      glm::vec3 computeBSDF(const glm::vec3 &l, const glm::vec3 &v)
      {
        return this->material->evaluateBSDF(l, this->normal, this->uv, v);
      }

      float computePDF_BSDF(const glm::vec3 &in, const glm::vec3 &out)
      {
        float pdf = 0.0f;
        this->material->getPDF(in, this->normal, this->uv, out, pdf);
        return pdf;
      }

      glm::vec3 computeBounceIncomingBSDF()
      {
        return this->computeBSDF(this->bounceRay.ray.direction, -this->incoming.direction);
      }

      float getBounceIncomingPDF()
      {
        return this->bounceRay.PDF;
      }

      glm::vec3 computeEmittance()
      {
        dispatchers::EmittanceDispatcher ed;
        this->object->accept(&ed);
        return ed.getEmittance();
      }

      void computeLuminaireSample(
        scene::Scene *scene,
        random::RandomizationHelper &randHelper
      )
      {
        scene->luminaireSample(bounceRay.ray.origin, normal, randHelper, this->lumSmpl);
      }

      const scene::Scene::LuminaireSample &getLuminaireSample()
      {
        return this->lumSmpl;
      }

      glm::vec3 computeLuminaireIncomingBSDF()
      {
        return this->computeBSDF(this->lumSmpl.direction, -this->incoming.direction);
      }

      float computeLuminairePDF(scene::Scene *scene)
      {
        return scene->getLuminairePDF(this->object, this->incoming, this->position, this->normal);
      }

    private:

      // vertex situation

      geometry::Ray incoming;

      glm::vec3 position;

      glm::vec3 normal;

      glm::vec2 uv;

      random::RandomRay bounceRay;

      scene::Scene::LuminaireSample lumSmpl;

      // additional info

      geometry::Object *object;

      materials::Material *material;
      
    };
  }
}

#endif