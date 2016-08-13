#include "utility/RenderedData.h"
#include "utility/Window.h"
#include "utility/ColorTools.hpp"

using namespace ray_storm::utility;

RenderedData::RenderedData()
{
  this->window = nullptr;
}

RenderedData::RenderedData(uint width, uint height)
{
  this->window = nullptr;
  this->initialize(width, height);
}

void RenderedData::initialize(uint width, uint height)
{
  this->data = cv::Mat::zeros(cv::Size(width, height), CV_32FC3);
}

void RenderedData::setPixel(int x, int y, const glm::vec3 &rgb)
{
  cv::Vec3f &pixel = this->data.at<cv::Vec3f>(cv::Point(x, y));

  // open cv uses BGR
  pixel[0] = rgb.b;
  pixel[1] = rgb.g;
  pixel[2] = rgb.r;
}

void RenderedData::setPixelSRGB(int x, int y, const glm::vec3 &rgbLinear)
{
  this->setPixel(x, y, ColorTools::linearToSRGB(rgbLinear));
}

void RenderedData::setWindow(Window *window)
{
  this->window = window;
}

const cv::Mat &RenderedData::getData()
{
  return this->data;
}

void RenderedData::signalChanged()
{
  if (this->window != nullptr)
  {
    this->window->refresh();
  }
}

uint RenderedData::getWidth() const
{
  return this->data.cols;
}

uint RenderedData::getHeight() const
{
  return this->data.rows;
}