#ifndef RENDERED_DATA_H_
#define RENDERED_DATA_H_

#include <memory>

#include <opencv2/core/core.hpp>

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"

#include "utility/Observable.h"

namespace ray_storm
{
  namespace utility
  {

    class Window;

    class RenderedData : public Observable
    {

    public:

      void setData(const cv::Mat &data);

      const cv::Mat &getData();
      
    private:

      cv::Mat data;

    };

    typedef std::shared_ptr<RenderedData> RenderedDataPtr;
  }
}

#endif