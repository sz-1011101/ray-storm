#ifndef ABSTRACT_FLAT_LENS_H_
#define ABSTRACT_FLAT_LENS_H_

#include <vector>
#include <memory>

#include "utility/common.hpp"
#include "camera/AbstractFlatLens.h"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace camera
  {
    class AbstractFlatLens
    {
    public:
      virtual ~AbstractFlatLens() {};

      virtual void spawnPoints(
        std::size_t amount,
        std::vector<glm::vec2> &lensPoints,
        random::RandomizationHelper &randHelper
      ) = 0;
      
    };

    typedef std::shared_ptr<AbstractFlatLens> AbstractFlatLensPtr;
  }
}

#endif