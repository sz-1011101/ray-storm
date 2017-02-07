#ifndef OBJECT_H_
#define OBJECT_H_

#include <memory>

#include "objects/Renderable.h"
#include "geometry/Intersectable.h"
#include "geometry/Bounded.hpp"
#include "geometry/ModifiableSurface.h"

namespace ray_storm
{

  namespace dispatchers
  {
    class AbstractObjectDispatcher;
  }

  namespace objects
  {

    class Object :
      public Renderable,
      public geometry::Intersectable<Object>,
      public geometry::Bounded,
      public geometry::ModifiableSurface
    {
    public: 

      Object(const materials::AbstractSVBxDFPtr &material) : Renderable(material)
      {
        
      }
      
      virtual ~Object() {};

      virtual void accept(dispatchers::AbstractObjectDispatcher *dispatcher) = 0;
      
    };

    typedef std::shared_ptr<Object> ObjectPtr;

  }
}

#endif