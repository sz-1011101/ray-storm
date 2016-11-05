#ifndef REFLECTOR_H_
#define REFLECTOR_H_

#include "objects/Object.h"

namespace ray_storm
{

  namespace dispatchers
  {
    class AbstractObjectDispatcher;
  }

  namespace objects
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