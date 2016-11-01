#ifndef SAMPLE_PLATE_H_
#define SAMPLE_PLATE_H_

#include <mutex>
#include <opencv2/core/core.hpp>

#include "utility/common.hpp"
#include "utility/ColorTools.hpp"

namespace ray_storm
{
  namespace utility
  {
    class SamplePlate
    {
    private:

      cv::Mat image;

      cv::Mat sampleCounter;

      std::mutex mutex;

    public:

      SamplePlate(uint32_t width, uint32_t height)
      {
        if (width != 0 && height != 0)
        {
          this->image = cv::Mat::zeros(cv::Size(width, height), CV_32FC3);
          this->sampleCounter = cv::Mat::zeros(cv::Size(width, height), CV_16UC1);
        }
      }

      void gatherSample(const glm::vec2 &xy, const glm::vec3 &rgb)
      {
        if (xy.x < 0.0f || xy.x >= 1.0f || xy.y < 0.0f || xy.y >= 1.0f)
        {
          return;
        }

        const cv::Point pnt(xy.x*this->image.cols, xy.y*this->image.rows);

        std::unique_lock<std::mutex> lock(this->mutex);
        cv::Vec3f &pixel = this->image.at<cv::Vec3f>(pnt);

        // open cv uses BGR
        pixel[0] += rgb.b;
        pixel[1] += rgb.g;
        pixel[2] += rgb.r;

        this->sampleCounter.at<ushort>(pnt)++;
      }

      void srgbMat(cv::Mat &mat)
      {
        mat = cv::Mat::zeros(image.size(), CV_32FC3);
        std::unique_lock<std::mutex> lock(this->mutex);
        for (int i = 0; i < this->image.rows; i++)
        {
          cv::Vec3f* imgPxl = this->image.ptr<cv::Vec3f>(i);
          ushort* cntPxl = this->sampleCounter.ptr<ushort>(i);
          cv::Vec3f* matPxl = mat.ptr<cv::Vec3f>(i);
          for (int j = 0; j < this->image.cols; j++)
          {
            if (cntPxl[j] > 0)
            {
              glm::vec3 rgbLinear(imgPxl[j][2], imgPxl[j][1], imgPxl[j][0]);
              rgbLinear /= static_cast<float>(cntPxl[j]);
              glm::vec3 rgbSRGB = ColorTools::linearToSRGB(rgbLinear);
              matPxl[j][0] = rgbSRGB.b;
              matPxl[j][1] = rgbSRGB.g;
              matPxl[j][2] = rgbSRGB.r;
            }
          }
        }
      }

      uint32_t getWidth()
      {
        return this->image.cols;
      }

      uint32_t getHeight()
      {
        return this->image.rows;
      }

    };
  }
}

#endif