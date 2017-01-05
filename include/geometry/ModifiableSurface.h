#ifndef MODIFIABLE_SURFACE_H_
#define MODIFIABLE_SURFACE_H_

#include "geometry/AbstractSurfaceNormalModifier.h"

namespace ray_storm
{
  namespace geometry
  {
    class ModifiableSurface
    {
    public:

      virtual ~ModifiableSurface() {};

      void setSurfaceNormalModifier(const AbstractSurfaceNormalModifierPtr &map);

      glm::vec3 modifyNormal(const glm::vec3 &n, const glm::vec2 &uv);

    private:

      AbstractSurfaceNormalModifierPtr map;

    };
  }
}


#endif