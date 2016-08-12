#include "renderer/AbstractRenderer.h"

using namespace ray_storm::renderer;

void AbstractRenderer::setRenderedData(utility::RenderedDataPtr &renderedData)
{
  this->renderedData = renderedData;
}