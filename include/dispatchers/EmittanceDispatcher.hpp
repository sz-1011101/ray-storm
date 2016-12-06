#ifndef EMITTANCE_DISPATCHER_H_
#define EMITTANCE_DISPATCHER_H_

#include "dispatchers/EmittanceDeciderDispatcher.hpp"
#include "objects/Emitter.h"
#include "objects/Reflector.h"

namespace ray_storm
{
  namespace dispatchers
  {

    class EmittanceDispatcher : public EmittanceDeciderDispatcher
    {

    public:

      EmittanceDispatcher(const glm::vec3 &l, const glm::vec3 &n, const glm::vec2 &uv) : l(l), n(n), uv(uv)
      {

      }

      void dispatch(objects::Emitter *emitter)
      {
        this->emitting = emitter->isEmitting();
        this->emittance = emitter->getEmittance(this->l, this->n, this->uv);
        this->PDF = this->emitting ? emitter->getPDF(this->l, this->n) : 0.0f;
      }
      
      void dispatch(objects::Reflector *reflector)
      {
        this->emitting = false;
        this->emittance = glm::vec3(0.0f);
        this->PDF = 0.0f;
      }

      glm::vec3 getEmittance() const
      {
        return this->emittance;
      }

      float getPDF() const
      {
        return this->PDF;
      }
      
    private:

      glm::vec3 l;
      
      glm::vec3 n;

      glm::vec2 uv;

      glm::vec3 emittance;

      float PDF;

    };
  }
}


#endif