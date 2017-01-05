#ifndef SURFACE_NORMAL_MODIFIER_FACTORY_H_
#define SURFACE_NORMAL_MODIFIER_FACTORY_H_

#include "geometry/AbstractSurfaceNormalModifier.h"
#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace geometry
  {
    class SurfaceNormalModifierFactory
    {
    private:

      SurfaceNormalModifierFactory() {}

    public:

      static AbstractSurfaceNormalModifierPtr createTextureBumpMap(
        const textures::Abstract2DTexturePtr<float> &bumpMap,
        float eps = 0.01f
      );
      
    };
  }
}

#endif