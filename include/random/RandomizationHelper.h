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

        MTEngine() : distribution(0.0f, 0.9999f) {};

        float draw()
        {
          return this->distribution(this->mtEngine);
        }

        int draw(int min, int max)
        {
          const float u = this->draw();
          const int range = max - min;
          return min + std::floor(u*range); // r in [a, b)
        }

      };

      inline float drawUniformRandom()
      {
        return this->engine.draw();
      }

      inline int drawUniformRandom(int min, int max)
      {
        return this->engine.draw(min, max);
      }

      glm::vec3 drawUniformRandomSphereDirection();

      glm::vec3 drawUniformRandomHemisphereDirection(const glm::vec3 &n);

      glm::vec3 drawCosineWeightedRandomHemisphereDirection(const glm::vec3 &n, float e);

      glm::vec3 drawCosineDistributedDirection(const glm::vec3 &n);

      static float uniformRandomSpherePDF()
      {
        return 1.0f/(4.0f*M_PI);
      }

      static float uniformRandomHemispherePDF()
      {
        return 1.0f/(2.0f*M_PI);
      }

      static float cosineRandomHemispherePDF(float cosTheta, float e);

    private:

      // the engine of this helper
      MTEngine engine;
      
      glm::vec3 transform(const glm::vec3 &n, const glm::vec3 &vector);

    };
  }
}

#endif