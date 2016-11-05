#include "objects/Renderable.h"

using namespace ray_storm::objects;

ray_storm::materials::Material *Renderable::getMaterial()
{
  return this->material.get();
}