#ifndef MODIFIABLE_SURFACE_H_
#define MODIFIABLE_SURFACE_H_

#include "geometry/AbstractSurfaceNormalModifier.h"
#include "geometry/SimpleIntersection.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class ModifiableSurface
    {
    public:

      virtual ~ModifiableSurface() {};

      void setSurfaceNormalModifier(const AbstractSurfaceNormalModifierPtr &map);

    protected:

      void modifyNormal(
        const glm::vec3 &direction,
        geometry::SimpleIntersection &intersection
      );

    private:

      AbstractSurfaceNormalModifierPtr map;

    };
  }
}


#endif