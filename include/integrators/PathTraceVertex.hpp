#ifndef PATH_TRACE_VERTEX_H_
#define PATH_TRACE_VERTEX_H_

#include "geometry/Ray.hpp"
#include "scene/Scene.h"
#include "random/RandomizationHelper.h"
#include "objects/Object.h"

namespace ray_storm
{
  namespace integrators
  {
    struct PathTraceVertex
    {
    public:

      PathTraceVertex(materials::SAMPLE_DIRECTION smplDir) : si(smplDir), object(nullptr), material(nullptr), delta(false) {}

      materials::SurfaceInteraction si;

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