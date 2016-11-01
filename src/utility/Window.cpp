#include <string>

#include "utility/Window.h"

const std::string WINDOW_NAME = "Render result";

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
  cv::waitKey(1);
}

void Window::wait()
{
  cv::waitKey(0);
}