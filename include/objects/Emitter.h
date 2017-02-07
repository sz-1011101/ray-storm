#ifndef EMITTER_H_
#define EMITTER_H_

#include "objects/Object.h"
#include "objects/Emitting.h"

namespace ray_storm
{

  namespace dispatchers
  {
    class AbstractObjectDispatcher;
  }

  namespace objects
  {

    class Emitter : public Object, public Emitting
    {
    public:

      Emitter(const materials::AbstractSVBxDFPtr &material, const glm::vec3 &emittance)
        : Object(material), Emitting(emittance) {};

      Emitter(const materials::AbstractSVBxDFPtr &material, const textures::Abstract2DTexturePtr<glm::vec3> &emittance)
      : Object(material), Emitting(emittance) {};

      virtual ~Emitter() {};

      void accept(dispatchers::AbstractObjectDispatcher *dispatcher);

    protected:

      static const float SURFACE_POINT_OFFSET;
      
    };

    typedef std::shared_ptr<Emitter> EmitterPtr;
  }
}


#endif