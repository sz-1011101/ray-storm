#ifndef DISTANCE_FIELD_FACTORY
#define DISTANCE_FIELD_FACTORY

#include "objects/Reflector.h"
#include "objects/Emitter.h"
#include "objects/Rectangle.hpp"

namespace ray_storm
{
  namespace objects
  {
    class ObjectFactory
    {

    public:

      static ReflectorPtr createDistSphere(
        const glm::vec3 &position, float radius, const materials::MaterialPtr &material
      );

      static ReflectorPtr createBox(
        const glm::vec3 &origin, const glm::vec3 &extends, const materials::MaterialPtr &material
      );

      static EmitterPtr createSphere(
        const glm::vec3 &position,
        float radius,
        const materials::MaterialPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

      static EmitterPtr createRectangle(
        const Rectangle::RectParams &rp,
        const materials::MaterialPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

    private:

      ObjectFactory() {}
      
    };
  }
}


#endif