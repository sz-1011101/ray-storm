#ifndef PATH_TRACE_VERTEX_H_
#define PATH_TRACE_VERTEX_H_

#include "geometry/Ray.hpp"
#include "scene/Scene.h"
#include "random/RandomizationHelper.h"
#include "objects/Object.h"
#include "materials/SurfaceInteraction.hpp"

namespace ray_storm
{
  namespace integrators
  {
    struct PathTraceVertex
    {
    public:

      PathTraceVertex(materials::SAMPLE_DIRECTION smplDir) : si(smplDir), object(nullptr) {}

      materials::SurfaceInteraction si;

      glm::vec3 reflected; // e.g. from brdf...

      glm::vec3 cummulative;

      objects::Object *object;
      
    };
  }
}

#endif