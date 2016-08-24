#ifndef ABSTRACT_BTDF_H_
#define ABSTRACT_BTDF_H_

#include <memory>

#include "random/RandomizationHelper.h"
#include "random/RandomDirection.hpp"

namespace ray_storm
{
  namespace materials
  {
    class AbstractBTDF
    {
    public:

      AbstractBTDF(float indexOfRefraction)
      {
        this->indexOfRefraction = indexOfRefraction;
      }

      virtual ~AbstractBTDF() {};

      virtual glm::vec3 evaluate(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec3 &v
      ) = 0;

      virtual void drawRefractedDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir
      ) = 0;

      virtual float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &out
      ) = 0;

    protected:

      float indexOfRefraction;
      
    };

    typedef std::shared_ptr<AbstractBTDF> AbstractBTDFPtr;
  }
}

#endif