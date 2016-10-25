#ifndef SUN_SKY_H_
#define SUN_SKY_H_

#include "utility/common.hpp"
#include "scene/AbstractSky.h"

namespace ray_storm
{
  namespace scene
  {
    class SunSky : public AbstractSky
    {
    public:

      SunSky(
        const glm::vec3 &skyColor,
        const glm::vec3 &sunColor,
        const glm::vec3 &sunDirection,
        float glareExponent
      ) : skyColor(skyColor), sunColor(sunColor),
        sunDirection(glm::normalize(sunDirection)), glareExponent(glareExponent)
      {

      }

      virtual ~SunSky() {};

      virtual glm::vec3 sample(const geometry::Ray &ray)
      {
        const float cosTheta = glm::dot(ray.direction, -this->sunDirection);

        return this->skyColor + std::pow(std::max(0.0f, cosTheta), glareExponent)*this->sunColor;
      }

    protected:

      glm::vec3 skyColor;

      glm::vec3 sunColor;

      glm::vec3 sunDirection;

      // similar to phong exponent
      float glareExponent;
      
    };

    typedef std::shared_ptr<SunSky> SunSkyPtr;
  }
}

#endif