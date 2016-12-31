#include "utility/RenderedData.h"
#include "utility/Window.h"

using namespace ray_storm::utility;

void RenderedData::setData(const cv::Mat &data)
{
  this->data = data;
  this->stateChanged();
}

const cv::Mat &RenderedData::getData()
{
  return this->data;
}

void RenderedData::writeToDisk(const std::string &filename)
{
  if (!this->data.empty())
  {
    cv::Mat img;
    this->data.convertTo(img, CV_8UC3, 255.0f);
    cv::imwrite(filename, img); 
  }
}