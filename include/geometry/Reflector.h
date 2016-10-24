#ifndef REFLECTOR_H_
#define REFLECTOR_H_

#include "geometry/Object.h"

namespace ray_storm
{

  namespace dispatchers
  {
    class AbstractObjectDispatcher;
  }

  namespace geometry
  {

    class Reflector : public Object
    {
    public:

      Reflector(const materials::MaterialPtr &material) : Object(material) {};
      
      virtual ~Reflector() {};

      void accept(dispatchers::AbstractObjectDispatcher *dispatcher);
      
    };

    typedef std::shared_ptr<Reflector> ReflectorPtr;
  }
}


#endif