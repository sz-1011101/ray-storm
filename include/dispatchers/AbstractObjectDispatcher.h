#ifndef ABSTRACT_OBJECT_DISPATCHER_H_
#define ABSTRACT_OBJECT_DISPATCHER_H_

#include "geometry/Emitter.h"
#include "geometry/Reflector.h"

namespace ray_storm
{
  namespace dispatchers
  {
    class AbstractObjectDispatcher
    {
    public:

      virtual ~AbstractObjectDispatcher() {};

      virtual void dispatch(geometry::Emitter *emitter) = 0;
      
      virtual void dispatch(geometry::Reflector *reflector) = 0;

      virtual void dispatch(const geometry::EmitterPtr &emitter) = 0;
      
      virtual void dispatch(const geometry::ReflectorPtr &reflector) = 0;

    };
  }
}


#endif