#ifndef DISTRIBUTIONS_H_
#define DISTRIBUTIONS_H_

#include <random>

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"

namespace ray_storm
{
  namespace random
  {

    class RandomizationHelper
    {
    public:

      struct MTEngine
      {
        std::mt19937 mtEngine;
        std::uniform_real_distribution<float> distribution;

        MTEngine() : distribution(0.0f, 1.0f) {};

        float draw()
        {
          return this->distribution(this->mtEngine);
        }

      };

      glm::vec3 drawUniformRandomSphereDirection();

      glm::vec3 drawUniformRandomHemisphereDirection(const glm::vec3 &n);

      glm::vec3 drawCosineWeightedRandomHemisphereDirection(const glm::vec3 &n, float e);

      float uniformRandomHemisphereInversePDF()
      {
        return 2.0f/M_PI;
      }

      float cosineRandomHemisphereInversePDF(float cosTheta, float e);

    private:

      // the engine of this helper
      MTEngine engine;
      
      glm::vec3 transform(const glm::vec3 &n, const glm::vec3 &vector);

    };
  }
}

#endif