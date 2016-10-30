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
    struct PathTraceVertex
    {
    public:

      PathTraceVertex() : object(nullptr), material(nullptr) {}

      // vertex situation

      glm::vec3 in;

      glm::vec3 position;

      glm::vec3 normal;

      glm::vec2 uv;

      glm::vec3 outPosition;

      glm::vec3 out;

      glm::vec3 bsdf;

      float bsdfPDF;

      glm::vec3 cummulative;

      geometry::Object *object;

      materials::Material *material;
      
    };
  }
}

#endif