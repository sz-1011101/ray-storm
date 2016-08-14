#ifndef IMAGE_H_
#define IMAGE_H_

#include <opencv2/core/core.hpp>
#include "utility/common.hpp"
#include "utility/ColorTools.hpp"

namespace ray_storm
{
  namespace utility
  {
    struct Image
    {
      cv::Mat image;

      Image(uint32_t width, uint32_t height)
      {
        this->image = cv::Mat::zeros(cv::Size(width, height), CV_32FC3);
      }

      inline void setPixel(int x, int y, const glm::vec3 &rgb)
      {
        cv::Vec3f &pixel = this->image.at<cv::Vec3f>(cv::Point(x, y));

        // open cv uses BGR
        pixel[0] = rgb.b;
        pixel[1] = rgb.g;
        pixel[2] = rgb.r;
      }

      inline void setPixelSRGB(int x, int y, const glm::vec3 &rgbLinear)
      {
        this->setPixel(x, y, ColorTools::linearToSRGB(rgbLinear));
      }

    };

  }
}
#endif