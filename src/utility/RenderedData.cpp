#include "utility/RenderedData.h"
#include "utility/Window.h"

using namespace ray_storm::utility;

RenderedData::RenderedData()
{
  this->window = nullptr;
}

void RenderedData::setData(const cv::Mat &data)
{
  this->data = data;
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