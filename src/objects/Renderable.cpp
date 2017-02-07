#include "objects/Renderable.h"

using namespace ray_storm::objects;

ray_storm::materials::AbstractSVBxDF *Renderable::getMaterial()
{
  return this->material.get();
}