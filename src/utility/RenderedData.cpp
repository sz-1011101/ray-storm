#include "utility/RenderedData.h"
#include "utility/Window.h"

using namespace ray_storm::utility;

RenderedData::RenderedData(uint32_t width, uint32_t height) : data(width, height)
{
  this->window = nullptr;
}

void RenderedData::setPixel(int x, int y, const glm::vec3 &rgb)
{
  this->data.setPixel(x, y, rgb);
}

void RenderedData::setPixelSRGB(int x, int y, const glm::vec3 &rgbLinear)
{
  this->data.setPixelSRGB(x, y, rgbLinear);
}

void RenderedData::setTile(int xOrg, int yOrg, Image &tile)
{
  tile.image.copyTo(this->data.image(cv::Rect(xOrg, yOrg, tile.image.cols, tile.image.rows)));
}

void RenderedData::setWindow(Window *window)
{
  this->window = window;
}

const ray_storm::utility::Image &RenderedData::getData()
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

uint32_t RenderedData::getWidth() const
{
  return this->data.image.cols;
}

uint32_t RenderedData::getHeight() const
{
  return this->data.image.rows;
}