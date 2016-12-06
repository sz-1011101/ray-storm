#ifndef DISTANCE_FIELD_FACTORY
#define DISTANCE_FIELD_FACTORY

#include "objects/Reflector.h"

namespace ray_storm
{
  namespace objects
  {
    class DistanceFieldFactory
    {

    public:

      ReflectorPtr createSphere(
        const glm::vec3 &position, float radius, const materials::MaterialPtr &material
      );

    private:

      DistanceFieldFactory() {}
      
      
    };
  }
}


#endif