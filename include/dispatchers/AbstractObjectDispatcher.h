#ifndef ABSTRACT_OBJECT_DISPATCHER_H_
#define ABSTRACT_OBJECT_DISPATCHER_H_

namespace ray_storm
{

  namespace geometry
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

      virtual void dispatch(geometry::Emitter *emitter) = 0;
      
      virtual void dispatch(geometry::Reflector *reflector) = 0;

    };
  }
}


#endif