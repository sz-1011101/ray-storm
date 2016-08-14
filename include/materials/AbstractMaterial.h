#ifndef ABSTRACT_MATERIAL_H_
#define ABSTRACT_MATERIAL_H_

#include <memory>

#include "utility/linear_alg.hpp"
#include "geometry/Ray.hpp"
#include "geometry/SimpleIntersection.hpp"
#include "random/RandomRay.hpp"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace materials
  {

    class AbstractMaterial
    {

    public:
      // brdf assumes vectors point towards the "source"
      virtual ~AbstractMaterial() {};

      virtual glm::vec3 evaluateBRDF(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v) = 0;

      virtual glm::vec3 getEmittance() = 0;

      virtual void drawReflectedRay(const glm::vec3 &in, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay) = 0;

    };

    typedef std::shared_ptr<materials::AbstractMaterial> AbstractMaterialPtr;
  }
}

#endif