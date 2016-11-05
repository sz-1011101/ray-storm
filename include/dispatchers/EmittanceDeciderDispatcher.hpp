#ifndef EMITTANCE_DECIDER_DISPATCHER_H_
#define EMITTANCE_DECIDER_DISPATCHER_H_

#include "dispatchers/AbstractObjectDispatcher.h"
#include "objects/Emitter.h"
#include "objects/Reflector.h"

namespace ray_storm
{
  namespace dispatchers
  {

    class EmittanceDeciderDispatcher : public AbstractObjectDispatcher
    {

    public:

      ~EmittanceDeciderDispatcher() {};

      virtual void dispatch(objects::Emitter *emitter)
      {
        this->emitting = emitter->isEmitting();
      }
      
      virtual void dispatch(objects::Reflector *reflector)
      {
        this->emitting = false;
      }

      bool isEmitting() const
      {
        return this->emitting;
      }

    protected:

      bool emitting;

    };
  }
}

#endif