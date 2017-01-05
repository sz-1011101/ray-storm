#include "geometry/ModifiableSurface.h"
#include "utility/common.hpp"

using namespace ray_storm::geometry;

void ModifiableSurface::setSurfaceNormalModifier(const AbstractSurfaceNormalModifierPtr &map)
{
  this->map = map;
}

void ModifiableSurface::modifyNormal(geometry::SimpleIntersection &intersection)
{
  if (this->map == nullptr)
  {
    return;
  }

  intersection.normal = this->map->modifyNormal(intersection.normal, intersection.texCoords);
}