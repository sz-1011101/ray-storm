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

      void setData(const cv::Mat &data);

      void setWindow(Window *window);

      const cv::Mat &getData();

      void signalChanged();

    private:

      cv::Mat data;
      
      Window *window;

    };

    typedef std::shared_ptr<RenderedData> RenderedDataPtr;
  }
}

#endif