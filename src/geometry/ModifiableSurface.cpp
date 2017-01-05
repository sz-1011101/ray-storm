#include "geometry/ModifiableSurface.h"
#include "utility/common.hpp"

using namespace ray_storm::geometry;

void ModifiableSurface::setSurfaceNormalModifier(const AbstractSurfaceNormalModifierPtr &map)
{
  this->map = map;
}

void ModifiableSurface::modifyNormal(
  const glm::vec3 &direction,
  geometry::SimpleIntersection &intersection
)
{
  if (this->map == nullptr)
  {
    return;
  }
  const glm::vec3 modiN = this->map->modifyNormal(intersection.normal, intersection.texCoords);
  if (std::signbit(glm::dot(direction, intersection.normal)) == std::signbit(glm::dot(direction, modiN)))
  {
    intersection.normal = modiN;
  }
}