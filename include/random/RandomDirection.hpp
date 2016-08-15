#ifndef RANDOM_DIRECTION_H_
#define RANDOM_DIRECTION_H_

#include "utility/linear_alg.hpp"

namespace ray_storm
{
  namespace random
  {
    struct RandomDirection
    {
      glm::vec3 direction;
      float inversePDF;
    };
  }
}
#endif