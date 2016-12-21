#ifndef RENDER_JOB_H_
#define RENDER_JOB_H_

#include "utility/common.hpp"

namespace ray_storm
{
  namespace renderer
  {
    struct RenderJob
    {

      RenderJob(uint32_t id, uint32_t xOrigin, uint32_t yOrigin, uint32_t width, uint32_t height)
      {
        this->id = id;
        this->xOrigin = xOrigin;
        this->yOrigin = yOrigin;
        this->width = width;
        this->height = height;
      }

      uint32_t id;

      uint32_t xOrigin;
      uint32_t yOrigin;

      uint32_t width;
      uint32_t height;

    };
  }
}


#endif