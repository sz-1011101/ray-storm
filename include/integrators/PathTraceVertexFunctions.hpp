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
        return vertex.object->getMaterial() != nullptr;
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
        return true;
      }

      static bool bounce(
        random::RandomizationHelper &randHelper,
        PathTraceVertex &vertex
      )
      {

        vertex.object->getMaterial()->sample(randHelper, vertex.si);
        vertex.bsdf = vertex.object->getMaterial()->evaluate(vertex.si);

        return true;
      }

      static glm::vec3 evaluate(const glm::vec3 &l, const PathTraceVertex &vertex, const glm::vec3 &v)
      {
        materials::SurfaceInteraction si(vertex.si.direction);
        si.l = l;
        si.v = v;
        si.n = vertex.si.n;
        si.uv = vertex.si.uv;
        si.x = vertex.si.x;
        return vertex.object->getMaterial()->evaluate(si);
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

      static float pdf(const glm::vec3 &in, const PathTraceVertex &vertex, const glm::vec3 &out)
      {
        materials::SurfaceInteraction si(vertex.si.direction);
        si.setIn(in);
        si.setOut(out);
        si.n = vertex.si.n;
        si.uv = vertex.si.uv;
        si.x = vertex.si.x;
        vertex.object->getMaterial()->pdf(si);
        return si.PDF;
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