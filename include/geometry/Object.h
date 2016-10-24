#ifndef OBJECT_H_
#define OBJECT_H_

#include <memory>

#include "geometry/Renderable.h"
#include "geometry/Intersectable.h"

namespace ray_storm
{

  namespace dispatchers
  {
    class AbstractObjectDispatcher;
  }

  namespace geometry
  {

    

    class Object : public Renderable, public Intersectable<Object>
    {
    public: 

      Object(const materials::MaterialPtr &material) : Renderable(material)
      {
        
      }
      
      virtual ~Object() {};

      virtual void accept(dispatchers::AbstractObjectDispatcher *dispatcher) = 0;
      
    };

    typedef std::shared_ptr<Object> ObjectPtr;

  }
}

#endif