#include "geometry/SurfaceNormalModifierFactory.h"
#include "geometry/BumpMap.hpp"

using namespace ray_storm::geometry;

AbstractSurfaceNormalModifierPtr SurfaceNormalModifierFactory::createTextureBumpMap(
  const textures::Abstract2DTexturePtr<float> &bumpMap,
  float eps
)
{
  return AbstractSurfaceNormalModifierPtr(new BumpMap(bumpMap, eps));
}