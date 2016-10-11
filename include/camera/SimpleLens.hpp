#ifndef SIMPLE_LENS_H_
#define SIMPLE_LENS_H_

#include <vector>

#include "utility/common.hpp"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace camera
  {

    class SimpleLens
    {
    public:

      SimpleLens(
        float diameter
      ) : diameter(diameter), radius(diameter/2.0f)
      {

      }

      void spawnPoints(
        std::size_t amount,
        std::vector<glm::vec2> &lensPoints,
        random::RandomizationHelper &randHelper
      )
      {
        // FIXME give as param
        lensPoints.clear();
        lensPoints.reserve(amount);
        while (amount > lensPoints.size())
        {
          glm::vec2 randPnt(randHelper.drawUniformRandom(), randHelper.drawUniformRandom());
          randPnt = 2.0f*randPnt - 1.0f;
          if (glm::length(randPnt) < 1.0f)
          {
            lensPoints.push_back(randPnt*this->radius);
          }
        }
      }

      float diameter;
      float radius;
      
    };
  }
}

#endif