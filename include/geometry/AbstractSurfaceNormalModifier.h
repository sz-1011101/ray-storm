#ifndef ABSTRACT_SURFACE_NORMAL_MODIFIER_H_
#define ABSTRACT_SURFACE_NORMAL_MODIFIER_H_

#include <memory>

#include "utility/common.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class AbstractSurfaceNormalModifier
    {
    public:

      virtual ~AbstractSurfaceNormalModifier() {};

      virtual glm::vec3 modifyNormal(const glm::vec3 &n, const glm::vec2 &uv) = 0;
      
    };

    typedef std::shared_ptr<AbstractSurfaceNormalModifier> AbstractSurfaceNormalModifierPtr;
  }
}

#endif