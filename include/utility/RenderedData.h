#ifndef RENDERED_DATA_H_
#define RENDERED_DATA_H_

#include <memory>

#include <opencv2/core/core.hpp>

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"
#include "utility/Image.hpp"

namespace ray_storm
{
  namespace utility
  {

    class Window;

    class RenderedData
    {

    public:

      RenderedData(uint32_t width, uint32_t height);

      void setPixel(int x, int y, const glm::vec3 &rgb);

      void setPixelSRGB(int x, int y, const glm::vec3 &rgbLinear);

      void setTile(int xOrg, int yOrg, Image &tile);

      void setWindow(Window *window);

      const ray_storm::utility::Image &getData();

      uint32_t getWidth() const;

      uint32_t getHeight() const;

      void signalChanged();

    private:

      Image data;
      
      Window *window;

    };

    typedef std::shared_ptr<RenderedData> RenderedDataPtr;
  }
}

#endif