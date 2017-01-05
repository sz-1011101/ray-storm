#include "geometry/ModifiableSurface.h"
#include "utility/common.hpp"

using namespace ray_storm::geometry;

void ModifiableSurface::setSurfaceNormalModifier(const AbstractSurfaceNormalModifierPtr &map)
{
  this->map = map;
}

glm::vec3 ModifiableSurface::modifyNormal(const glm::vec3 &n, const glm::vec2 &uv)
{
  if (this->map == nullptr)
  {
    return n;
  }

  return this->map->modifyNormal(n, uv);
}