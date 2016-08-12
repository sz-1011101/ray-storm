#include "utility/RenderedData.h"
#include "utility/Window.h"

using namespace ray_storm::utility;

RenderedData::RenderedData(Window *window)
{
  this->window = window;
}

void RenderedData::initialize(int width, int height)
{
  this->data = cv::Mat::zeros(cv::Size(width, height), CV_32FC3);
}

void RenderedData::setPixel(int x, int y, const glm::vec3 &rgb)
{

}

const cv::Mat &RenderedData::getData()
{
  return this->data;
}

void RenderedData::signalChanged()
{
  // TODO do something on the window
}