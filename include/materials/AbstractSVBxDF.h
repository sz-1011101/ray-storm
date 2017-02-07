#ifndef ABSTRACT_SVBXDF_H_
#define ABSTRACT_SVBXDF_H_

#include <memory>

#include "random/RandomizationHelper.h"
#include "materials/SurfaceInteraction.hpp"

namespace ray_storm
{
  namespace materials
  {
    class AbstractSVBxDF
    {
    public:

      virtual ~AbstractSVBxDF() {};

      virtual glm::vec3 evaluate(
        const SurfaceInteraction &si
      ) = 0;

      virtual void sample(
        random::RandomizationHelper &randHelper,
        SurfaceInteraction &si
      ) = 0;

      virtual void pdf(
        SurfaceInteraction &si
      ) = 0;

    };

    typedef std::shared_ptr<AbstractSVBxDF> AbstractSVBxDFPtr;
  }
}

#endif