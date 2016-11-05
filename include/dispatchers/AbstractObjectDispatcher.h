#ifndef ABSTRACT_OBJECT_DISPATCHER_H_
#define ABSTRACT_OBJECT_DISPATCHER_H_

namespace ray_storm
{

  namespace objects
  {
    class Emitter;

    class Reflector; 
  }

  namespace dispatchers
  {

    class AbstractObjectDispatcher
    {
    public:

      virtual ~AbstractObjectDispatcher() {};

      virtual void dispatch(objects::Emitter *emitter) = 0;
      
      virtual void dispatch(objects::Reflector *reflector) = 0;

    };
  }
}


#endif