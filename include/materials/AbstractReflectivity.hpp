#ifndef ABSTRACT_REFLECTIVITY_H_
#define ABSTRACT_REFLECTIVITY_H_

#include <memory>

#include "utility/common.hpp"

namespace ray_storm
{
  namespace materials
  {
    class AbstractReflectivity
    {
    public:

      virtual ~AbstractReflectivity() {};

      virtual float computeF(float eta1, float eta2, const glm::vec3 &in, const glm::vec3 &n) = 0;
      
    };

    typedef std::shared_ptr<AbstractReflectivity> AbstractReflectivityPtr;
  }
}

#endif