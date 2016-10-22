#include "camera/AbstractSingleImageCamera.h"

using namespace ray_storm::camera;

AbstractSingleImageCamera::AbstractSingleImageCamera(const utility::RenderedDataPtr &renderedData)
  : renderedData(renderedData)
{

}

void AbstractSingleImageCamera::setTile(int xOrg, int yOrg, utility::Image &tile)
{
  if (this->renderedData == nullptr)
  {
    return;
  }
  this->renderedData->setTile(xOrg, yOrg, tile);
  this->renderedData->signalChanged();
}

uint32_t AbstractSingleImageCamera::getWidth()
{
  return this->renderedData != nullptr ? this->renderedData->getWidth() : 0;
}

uint32_t AbstractSingleImageCamera::getHeight()
{
  return this->renderedData != nullptr ? this->renderedData->getHeight() : 0;
}