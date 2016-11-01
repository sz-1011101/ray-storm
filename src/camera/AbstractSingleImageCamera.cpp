#include "camera/AbstractSingleImageCamera.h"

using namespace ray_storm::camera;

AbstractSingleImageCamera::AbstractSingleImageCamera(const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height)
  : renderedData(renderedData), samples(width, height)
{

}

uint32_t AbstractSingleImageCamera::getWidth()
{
  return this->samples.getWidth();
}

uint32_t AbstractSingleImageCamera::getHeight()
{
  return this->samples.getHeight();
}

void AbstractSingleImageCamera::gatherSample(const glm::vec2 &xy, const glm::vec3 &sample)
{
  this->samples.gatherSample(xy, sample);
}

void AbstractSingleImageCamera::signal()
{
  if (this->renderedData != nullptr)
  {
    cv::Mat img;
    this->samples.srgbMat(img);
    this->renderedData->setData(img);
    this->renderedData->signalChanged();
  }
}