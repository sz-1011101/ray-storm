#include <string>

#include "utility/Window.h"

const std::string WINDOW_NAME = "ray-storm";

using namespace ray_storm::utility;

Window::Window()
{
  cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
}

Window::~Window()
{
  cv::destroyWindow(WINDOW_NAME);
}

void Window::setRenderedData(RenderedDataPtr renderedData)
{
  this->renderedData = renderedData;
}

void Window::refresh()
{
  cv::imshow(WINDOW_NAME, this->renderedData->getData());
  cv::updateWindow(WINDOW_NAME);
  cv::waitKey(5);
}

void Window::wait()
{
  cv::waitKey(0);
}