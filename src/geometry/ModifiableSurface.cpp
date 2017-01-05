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
  // it could happen that the normal is changed below the viewing direction's plane, which will produce ugly artifacts,
  // in this case, we just use the unmodified normal 
  if (std::signbit(glm::dot(direction, intersection.normal)) == std::signbit(glm::dot(direction, modiN)))
  {
    intersection.normal = modiN;
  }
}