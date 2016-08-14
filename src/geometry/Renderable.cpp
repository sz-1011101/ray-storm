#include "geometry/Renderable.h"

using namespace ray_storm::geometry;

ray_storm::materials::Material *Renderable::getMaterial()
{
  return this->material.get();
}