#ifndef OBJECT_H_
#define OBJECT_H_

#include <memory>

#include "geometry/Renderable.h"
#include "geometry/Intersectable.h"
#include "geometry/Emitting.h"

namespace ray_storm
{
  namespace geometry
  {

    class Object : public Renderable, public Intersectable<Object>, public Emitting
    {
    public:

      Object(const materials::MaterialPtr &material, const glm::vec3 &emittance) : Renderable(material), Emitting(emittance)
      {
        
      }
      
      virtual ~Object() {};

      virtual bool intersect(const Ray &ray, Intersection<Object> &intersection) = 0;

      virtual float getSurfaceArea() = 0;

      virtual float getPDF() = 0;

      virtual glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper) = 0;
      
    };

    typedef std::shared_ptr<Object> ObjectPtr;

  }
}

#endif