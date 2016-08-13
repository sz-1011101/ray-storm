#ifndef RENDERED_DATA_H_
#define RENDERED_DATA_H_

#include <memory>

#include <opencv2/core/core.hpp>

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"

namespace ray_storm
{
  namespace utility
  {

    class Window;

    class RenderedData
    {

    public:

      RenderedData();

      RenderedData(uint width, uint height);

      void initialize(uint width, uint height);

      void setPixel(int x, int y, const glm::vec3 &rgb);

      void setPixelSRGB(int x, int y, const glm::vec3 &rgbLinear);

      void setWindow(Window *window);

      const cv::Mat &getData();

      uint getWidth() const;

      uint getHeight() const;

      void signalChanged();

    private:

      cv::Mat data;
      
      Window *window;

    };

    typedef std::shared_ptr<RenderedData> RenderedDataPtr;
  }
}

#endif