#ifndef SURFACE_INTERACTION_H_
#define SURFACE_INTERACTION_H_

#include "utility/common.hpp"
#include "materials/LIGHT_INTERACTION_TYPE.h"
#include "materials/SAMPLE_DIRECTION.h"

namespace ray_storm
{
  namespace materials
  {
    class SurfaceInteraction
    {

    public:

      SAMPLE_DIRECTION direction;

      glm::vec3 l;
      glm::vec3 x;
      glm::vec3 n;
      glm::vec2 uv;
      glm::vec3 v;

      LIGHT_INTERACTION_TYPE type;
      float reflectivity;

      float PDF;
      bool delta;

      bool sampled;

      SurfaceInteraction(
        SAMPLE_DIRECTION direction
      ) : direction(direction), type(REFLECTION), reflectivity(1.0f), delta(false), sampled(false)
      {

      }

      void setIn(const glm::vec3 &in)
      {
        if (direction == SAMPLE_DIRECTION::V_TO_L)
        {
          this->v = -in;
        }
        else if (direction == SAMPLE_DIRECTION::L_TO_V)
        {
          this->l = -in;
        }
      }

      void setOut(const glm::vec3 &out)
      {
        if (direction == SAMPLE_DIRECTION::V_TO_L)
        {
          this->l = out;
        }
        else if (direction == SAMPLE_DIRECTION::L_TO_V)
        {
          this->v = out;
        }
      }

      void offset()
      {
        const float RAY_OFFSET_EPSILON = 0.005f;
        const glm::vec3 in = this->getIn();
        switch (this->type)
        {
          case REFLECTION:
            this->x += glm::dot(this->n, in) > 0.0f ? -RAY_OFFSET_EPSILON*this->n : RAY_OFFSET_EPSILON*this->n;
            break;
          case REFRACTION:
            this->x += glm::dot(this->n, in) > 0.0f ? RAY_OFFSET_EPSILON*this->n : -RAY_OFFSET_EPSILON*this->n;
            break;
        }
      }

      glm::vec3 getIn() const
      {
        return this->direction == SAMPLE_DIRECTION::V_TO_L ? -v : -l;
      }

      glm::vec3 getOut() const
      {
        return this->direction == SAMPLE_DIRECTION::V_TO_L ? l : v;
      }

      void finalizeSampling() {
        this->offset();
        this->sampled = true;
      }

    };
  }
}

#endif