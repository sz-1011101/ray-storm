#ifndef DEFAULT_OBJECT_DISPATCHER_H_
#define DEFAULT_OBJECT_DISPATCHER_H_

#include "dispatchers/AbstractObjectDispatcher.h"
#include "geometry/Emitter.h"
#include "geometry/Reflector.h"

namespace ray_storm
{
  namespace dispatchers
  {

    class EmittanceDispatcher : public AbstractObjectDispatcher
    {

    public:

      void dispatch(geometry::Emitter *emitter)
      {
        this->emittance = emitter->getEmittance();
        this->emitting = true;
        this->PDF = emitter->getPDF();
      }
      
      void dispatch(geometry::Reflector *reflector)
      {
        this->emittance = glm::vec3(0.0f);
        this->emitting = true;
        this->PDF = 0.0f;
      }

      void dispatch(const geometry::EmitterPtr &emitter)
      {
        this->emittance = emitter->getEmittance();
        this->emitting = true;
        this->PDF = emitter->getPDF();
      }
      
      void dispatch(const geometry::ReflectorPtr &reflector)
      {
        this->emittance = glm::vec3(0.0f);
        this->emitting = false;
        this->PDF = 0.0f;
      }

      glm::vec3 getEmittance() const
      {
        return this->emittance;
      }

      bool isEmitting() const
      {
        return this->emitting;
      }

      float getPDF() const
      {
        return this->PDF;
      }
      
    private:

      glm::vec3 emittance;

      bool emitting;

      float PDF;

    };
  }
}


#endif