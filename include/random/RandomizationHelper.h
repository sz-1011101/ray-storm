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
    private:
      
      RandomizationHelper() {};

      static glm::vec3 transform(const glm::vec3 &n, const glm::vec3 &vector);
      
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

      static glm::vec3 drawUniformRandomSphereDirection(MTEngine &engine);

      static glm::vec3 drawUniformRandomHemisphereDirection(MTEngine &engine, const glm::vec3 &n);

      static glm::vec3 drawCosineWeightedRandomHemisphereDirection(MTEngine &engine, const glm::vec3 &n, float e);

      static float uniformRandomHemisphereInversePDF()
      {
        return 2.0f/M_PI;
      }

      static float cosineRandomHemisphereInversePDF(float cosTheta, float e);

    };
  }
}

#endif