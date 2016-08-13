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

      virtual ~AbstractMaterial() {};

      virtual glm::vec3 evaluateBRDF(const glm::vec3 &v, 
        const glm::vec3 &n, const glm::vec3 &l) = 0;

      virtual glm::vec3 getEmittance() = 0;

      virtual void drawReflectedRay(const glm::vec3 &v, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper::MTEngine &engine, random::RandomRay &randRay) = 0;

    };

    typedef std::shared_ptr<materials::AbstractMaterial> AbstractMaterialPtr;
  }
}

#endif