#ifndef WINDOW_H_
#define WINDOW_H_

#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "utility/common.hpp"
#include "utility/RenderedData.h"
#include "utility/Observer.h"

namespace ray_storm
{
  namespace utility
  {
    class Window : public Observer
    {
    public:
      
      Window();
      
      ~Window();

      void set(const RenderedDataPtr &renderedData);

      void wait();

      void stateChanged();

    private:

      RenderedDataPtr renderedData;
      
    };

    typedef std::shared_ptr<Window> WindowPtr;
  }
}

#endif