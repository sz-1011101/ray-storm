#ifndef ABSTRACT_BXDF_H_
#define ABSTRACT_BXDF_H_

#include <memory>

#include "random/RandomizationHelper.h"
#include "random/RandomDirection.hpp"

namespace ray_storm
{
  namespace materials
  {
    class AbstractSVBxDF
    {
    public:

      virtual ~AbstractSVBxDF() {};

      virtual glm::vec3 evaluate(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &v
      ) = 0;

      virtual void drawDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper,
        random::RandomDirection &randDir
      ) = 0;

      virtual float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &out
      ) = 0;
    };

    typedef std::shared_ptr<AbstractSVBxDF> AbstractSVBxDFPtr;
  }
}

#endif