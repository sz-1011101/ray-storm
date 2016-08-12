#ifndef ABSTRACT_MATERIAL_H_
#define ABSTRACT_MATERIAL_H_

#include <memory>

#include "utility/linear_alg.hpp"
#include "geometry/Ray.hpp"
#include "geometry/SimpleIntersection.hpp"

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

    };

    typedef std::shared_ptr<materials::AbstractMaterial> AbstractMaterialPtr;
  }
}

#endif