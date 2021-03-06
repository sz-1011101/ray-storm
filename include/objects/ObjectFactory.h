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
        const glm::vec3 &position, float radius, const materials::AbstractSVBxDFPtr &material
      );

      static ReflectorPtr createSphereUnion(
        const glm::vec3 &position, const materials::AbstractSVBxDFPtr &material
      );

      static ReflectorPtr createBox(
        const glm::vec3 &origin, const glm::vec3 &extends, const materials::AbstractSVBxDFPtr &material
      );

      static EmitterPtr createSphere(
        const glm::vec3 &position,
        float radius,
        const materials::AbstractSVBxDFPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

      static EmitterPtr createSphere(
        const glm::vec3 &position,
        float radius,
        const materials::AbstractSVBxDFPtr &material,
        const textures::Abstract2DTexturePtr<glm::vec3> &emittance
      );

      static EmitterPtr createRectangle(
        const Rectangle::RectParams &rp,
        const materials::AbstractSVBxDFPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

      static EmitterPtr createRectangle(
        const Rectangle::RectParams &rp,
        const materials::AbstractSVBxDFPtr &material,
        const textures::Abstract2DTexturePtr<glm::vec3> &emittance
      );

    private:

      ObjectFactory() {}
      
    };
  }
}


#endif