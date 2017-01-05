#ifndef PATH_TRACE_VERTEX_FUNCTIONS_H_
#define PATH_TRACE_VERTEX_FUNCTIONS_H_

#include "integrators/PathTraceVertex.hpp"
#include "dispatchers/EmittanceDispatcher.hpp"
#include "scene/Scene.h"

namespace ray_storm
{
  namespace integrators
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
        geometry::Intersection<objects::Object> isect;
        if (!scene->intersect(ray, isect))
        {
          return false;
        }
      
        vertex.si.setIn(ray.direction);
        vertex.si.x = isect.intersection.position;
        vertex.si.n = isect.intersection.normal;
        vertex.si.uv = isect.intersection.texCoords;
        vertex.object = isect.intersected;
        vertex.material = vertex.object->getMaterial();
        return true;
      }

      static bool bounce(
        random::RandomizationHelper &randHelper,
        PathTraceVertex &vertex
      )
      {
        random::RandomDirection dir;
        if (!vertex.material->sampleBSDF(vertex.si, randHelper, dir))
        {
          return false;
        }

        vertex.bsdf = vertex.material->evaluateBSDF(vertex.si);
        vertex.bsdfPDF = dir.PDF;
        vertex.delta = dir.delta;

        return true;
      }

      static glm::vec3 evaluateBSDF(const glm::vec3 &l, const PathTraceVertex &vertex, const glm::vec3 &v)
      {
        return vertex.material->evaluateBSDF(l, vertex.si.n, vertex.si.uv, v);
      }

      static glm::vec3 emittance(const PathTraceVertex &vertex)
      {
        dispatchers::EmittanceDispatcher ed(vertex.si.v, vertex.si.n, vertex.si.uv);
        vertex.object->accept(&ed);
        return ed.getEmittance();
      }

      static scene::Scene::LuminaireSample sampleLuminaire(const PathTraceVertex &vertex, scene::Scene *scene, random::RandomizationHelper &randHelper)
      {
        scene::Scene::LuminaireSample lSample;
        scene->sampleLuminaire(vertex.si.x, vertex.si.n, randHelper, lSample);
        return lSample;
      }

      static float bsdfPDF(const glm::vec3 &in, const PathTraceVertex &vertex, const glm::vec3 &out)
      {
        float pdf = 0.0f;
        vertex.material->getPDF(in, vertex.si.n, vertex.si.uv, out, pdf);
        return pdf;
      }

      static float luminarePDF(const glm::vec3 &position, const PathTraceVertex &vertex, scene::Scene *scene)
      {
        return scene->getLuminairePDF(vertex.object, geometry::Ray(position, vertex.si.getIn()), vertex.si.x, vertex.si.n, vertex.si.uv);
      }

    private:

      PathTraceVertexFunctions() {};
      
    };
  }
}

#endif