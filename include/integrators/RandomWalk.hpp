#ifndef RANDOM_WALK_H_
#define RANDOM_WALK_H_

#include "integrators/PathTraceVertex.hpp"

namespace ray_storm
{
  namespace integrators
  {
    struct RandomWalk
    {
      RandomWalk() : absorbed(true), length(0) {}

      bool absorbed;
      std::vector<PathTraceVertex> vertices;
      int length;
    };
  }
}

#endif


