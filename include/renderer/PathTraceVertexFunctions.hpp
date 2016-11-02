#ifndef PATH_TRACE_VERTEX_FUNCTIONS_H_
#define PATH_TRACE_VERTEX_FUNCTIONS_H_

#include "renderer/PathTraceVertex.hpp"
#include "scene/Scene.h"

namespace ray_storm
{
  namespace renderer
  {
    class PathTraceVertexFunctions
    {
    public:

      static bool isReflecting(PathTraceVertex &vertex)
      {
        return vertex.material != nullptr;
      }

      static bool intersect(const geometry::Ray &ray, scene::Scene *scene, PathTraceVertex &vertex)
      {
        geometry::Intersection<geometry::Object> isect;
        if (!scene->intersect(ray, isect))
        {
          return false;
        }
      
        vertex.in = ray.direction;
        vertex.position = isect.intersection.position;
        vertex.offPosition = vertex.position;
        vertex.normal = isect.intersection.normal;
        vertex.uv = isect.intersection.texCoords;
        vertex.object = isect.intersected;
        vertex.material = vertex.object->getMaterial();
        return true;
      }

      static bool bounce(
        random::RandomizationHelper &randHelper,
        PathTraceVertex &vertex
      )
      {
        random::RandomRay ray;
        if (!vertex.material->sampleBSDF(vertex.in, vertex.position, vertex.normal, vertex.uv, randHelper, ray))
        {
          return false;
        }

        vertex.out = ray.ray.direction;
        vertex.offPosition = ray.ray.origin;

        PathTraceVertexFunctions::delta(vertex);
        if (!vertex.delta)
        {
          vertex.bsdf = PathTraceVertexFunctions::evaluateBSDF(vertex.out, vertex, -vertex.in);
          vertex.bsdfPDF = ray.PDF;
        }
        else
        {
          vertex.bsdf = glm::vec3(1.0f);
          vertex.bsdfPDF = 1.0f;
        }

        return true;
      }

      static glm::vec3 evaluateBSDF(const glm::vec3 &l, const PathTraceVertex &vertex, const glm::vec3 &v)
      {
        return vertex.material->evaluateBSDF(l, vertex.normal, vertex.uv, v);
      }

      static glm::vec3 emittance(PathTraceVertex &vertex)
      {
        dispatchers::EmittanceDispatcher ed;
        vertex.object->accept(&ed);
        return ed.getEmittance();
      }

      static scene::Scene::LuminaireSample sampleLuminaire(const PathTraceVertex &vertex, scene::Scene *scene, random::RandomizationHelper &randHelper)
      {
        scene::Scene::LuminaireSample lSample;
        scene->sampleLuminaire(vertex.offPosition, vertex.normal, randHelper, lSample);
        return lSample;
      }

      static float bsdfPDF(const glm::vec3 &in, const PathTraceVertex &vertex, const glm::vec3 &out)
      {
        float pdf = 0.0f;
        vertex.material->getPDF(in, vertex.normal, vertex.uv, out, pdf);
        return pdf;
      }

      static float luminarePDF(const glm::vec3 &position, const PathTraceVertex &vertex, scene::Scene *scene)
      {
        return scene->getLuminairePDF(vertex.object, geometry::Ray(position, vertex.in), vertex.position, vertex.normal);
      }

      static void delta(PathTraceVertex &vertex)
      {
        vertex.delta = vertex.material->isDelta();
      }

    private:

      PathTraceVertexFunctions();
      
    };
  }
}

#endif