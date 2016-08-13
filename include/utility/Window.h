#ifndef WINDOW_H_
#define WINDOW_H_

#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "utility/common.hpp"
#include "utility/RenderedData.h"

namespace ray_storm
{
  namespace utility
  {
    class Window
    {
    public:
      
      Window();
      
      ~Window();

      void setRenderedData(RenderedDataPtr renderedData);

      void refresh();

      void wait();

    private:

      RenderedDataPtr renderedData;
      
    };
  }
}

#endif