#ifndef PATH_TRACER_H_
#define PATH_TRACER_H_

#include "utility/common.hpp"
#include "renderer/AbstractRenderer.h"
#include "utility/RenderedData.h"

namespace ray_storm
{
  namespace renderer
  {
    class PathTracer : public AbstractRenderer
    {

    public:

      void render(const scene::Scene &scene, camera::AbstractCameraPtr &camera);
      
    };
  }
}

#endif