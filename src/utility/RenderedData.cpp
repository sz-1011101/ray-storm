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