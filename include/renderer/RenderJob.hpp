#ifndef RENDER_JOB_H_
#define RENDER_JOB_H_

#include "utility/common.hpp"
#include "utility/Image.hpp"

namespace ray_storm
{
  namespace renderer
  {
    struct RenderJob
    {

      RenderJob(uint32_t id, uint32_t xOrigin, uint32_t yOrigin, uint32_t width, uint32_t height) : 
        tile(width, height)
      {
        this->id = id;
        this->xOrigin = xOrigin;
        this->yOrigin = yOrigin;
        this->width = width;
        this->height = height;
      }

      inline void setPixel(int x, int y, const glm::vec3 &rgb)
      {
        this->tile.setPixel(x, y, rgb);
      }

      inline void setPixelSRGB(int x, int y, const glm::vec3 &rgbLinear)
      {
        this->tile.setPixelSRGB(x, y, rgbLinear);
      }

      uint32_t id;

      uint32_t xOrigin;
      uint32_t yOrigin;

      uint32_t width;
      uint32_t height;

      utility::Image tile;

    };
  }
}


#endif