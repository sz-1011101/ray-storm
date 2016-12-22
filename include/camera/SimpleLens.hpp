#ifndef SIMPLE_LENS_H_
#define SIMPLE_LENS_H_

#include "camera/AbstractFlatLens.h"

namespace ray_storm
{
  namespace camera
  {

    class SimpleLens : public AbstractFlatLens
    {
    public:

      SimpleLens(
        float diameter
      ) : diameter(diameter), radius(diameter/2.0f)
      {

      }

      virtual ~SimpleLens() {};

      virtual void spawnPoints(
        std::size_t amount,
        std::vector<glm::vec2> &lensPoints,
        random::RandomizationHelper &randHelper
      )
      {
        lensPoints.clear();
        lensPoints.reserve(amount);
        for (std::size_t i = 0; i < amount; i++)
        {
          lensPoints.push_back(this->spawnPoint(randHelper));
        }
      }

      virtual glm::vec2 spawnPoint(random::RandomizationHelper &randHelper)
      {
        // http://mathworld.wolfram.com/DiskPointPicking.html
        const float rSqrt = this->radius*std::sqrt(randHelper.drawUniformRandom());
        const float theta = randHelper.drawUniformRandom()*2.0f*M_PI;
        return glm::vec2(rSqrt*std::cos(theta), rSqrt*std::sin(theta));
      }

      virtual bool onLens(const glm::vec2 &p)
      {
        return glm::length(p) < this->radius;
      }

      float diameter;
      float radius;
      
    };

    typedef std::shared_ptr<SimpleLens> SimpleLensPtr;
  }
}

#endif