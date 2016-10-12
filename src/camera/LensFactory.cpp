#include "camera/LensFactory.h"

using namespace ray_storm::camera;

ApertureLensPtr LensFactory::createNPolygon(std::size_t n, float diameter)
{
  std::size_t _n = 3 <= n ? n : 3;
  std::vector<glm::vec2> shape;
  shape.reserve(_n);
  const float radStep = 2.0f*M_PI/_n;
  const float radius = diameter/2.0f;
  for (std::size_t i = 0; i < _n; i++)
  {
    shape.push_back(glm::vec2(std::cos(radStep*i), std::sin(radStep*i))*radius);
  }

  return ApertureLensPtr(new ApertureLens(shape));
}