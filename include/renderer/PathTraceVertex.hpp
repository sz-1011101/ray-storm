#ifndef PATH_TRACE_VERTEX_H_
#define PATH_TRACE_VERTEX_H_

#include "geometry/Ray.hpp"
#include "scene/Scene.h"
#include "random/RandomizationHelper.h"
#include "objects/Object.h"

namespace ray_storm
{
  namespace renderer
  {
    struct PathTraceVertex
    {
    public:

      PathTraceVertex() : object(nullptr), material(nullptr), delta(false) {}

      // vertex situation

      glm::vec3 in;

      glm::vec3 position;

      glm::vec3 normal;

      glm::vec2 uv;

      // elevated pos for ray spawning
      glm::vec3 offPosition;

      glm::vec3 out;

      glm::vec3 bsdf;

      float bsdfPDF;

      glm::vec3 cummulative;

      objects::Object *object;

      materials::Material *material;

      bool delta;
      
    };
  }
}

#endif