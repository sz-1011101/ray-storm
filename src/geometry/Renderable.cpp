#include "geometry/Renderable.h"

using namespace ray_storm::geometry;

ray_storm::materials::AbstractMaterial *Renderable::getMaterial()
{
  return this->material.get();
}