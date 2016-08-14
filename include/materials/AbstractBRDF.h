#ifndef ABSTRACT_BRDF_H_
#define ABSTRACT_BRDF_H_

#include <memory>

#include "geometry/Ray.hpp"
#include "geometry/SimpleIntersection.hpp"
#include "random/RandomRay.hpp"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace materials
  {
    class AbstractBRDF
    {
    public:

      virtual ~AbstractBRDF() {};

      virtual glm::vec3 evaluate(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v) = 0;

      virtual void drawReflectedRay(const glm::vec3 &in, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay) = 0;
      
    };

    typedef std::shared_ptr<AbstractBRDF> AbstractBRDFPtr;

  }
}

#endif