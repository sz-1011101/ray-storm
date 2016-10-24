#ifndef EMITTER_H_
#define EMITTER_H_

#include "geometry/Object.h"
#include "geometry/Emitting.h"

namespace ray_storm
{

  namespace dispatchers
  {
    class AbstractObjectDispatcher;
  }

  namespace geometry
  {

    class Emitter : public Object, public Emitting
    {
    public:

      Emitter(const materials::MaterialPtr &material, const glm::vec3 &emittance)
        : Object(material), Emitting(emittance) {};

      virtual ~Emitter() {};

      void accept(dispatchers::AbstractObjectDispatcher *dispatcher);
      
    };

    typedef std::shared_ptr<Emitter> EmitterPtr;
  }
}


#endif