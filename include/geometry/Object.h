#ifndef OBJECT_H_
#define OBJECT_H_

#include <memory>

#include "geometry/Renderable.h"
#include "geometry/Intersectable.h"

namespace ray_storm
{
  namespace geometry
  {

    class Object : public Renderable, public Intersectable<Object>
    {
    public:

      Object(materials::MaterialPtr &material) : Renderable(material)
      {
        
      }
      
      virtual ~Object() {};

      virtual bool intersect(const Ray &ray, Intersection<Object> &intersection) = 0;
      
    };

    typedef std::shared_ptr<Object> ObjectPtr;

  }
}

#endif