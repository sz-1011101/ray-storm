#ifndef CONSTANT_SKY_H_
#define CONSTANT_SKY_H_

#include "scene/AbstractSky.h"

namespace ray_storm
{
  namespace scene
  {
    class ConstantSky : public AbstractSky
    {
    public:

      ConstantSky(const glm::vec3 &color) : color(color) {};

      virtual ~ConstantSky() {};

      virtual glm::vec3 sample(const geometry::Ray &ray)
      {
        return this->color;
      }

    protected:

      glm::vec3 color;
      
    };

    typedef std::shared_ptr<ConstantSky> ConstantSkyPtr;
  }
}

#endif