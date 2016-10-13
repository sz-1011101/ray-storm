#ifndef ABSTRACT_SKY_H_
#define ABSTRACT_SKY_H_

#include <memory>

#include "utility/common.hpp"
#include "geometry/Ray.hpp"

namespace ray_storm
{
  namespace scene
  {
    class AbstractSky
    {
    public:

      ~AbstractSky() {};

      virtual glm::vec3 sample(const geometry::Ray &ray) = 0;
      
    };

    typedef std::shared_ptr<AbstractSky> AbstractSkyPtr;
  }
}


#endif