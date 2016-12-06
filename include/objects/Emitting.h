#ifndef EMITTING_H_
#define EMITTING_H_

#include "random/RandomizationHelper.h"
#include "random/RandomRay.hpp"

namespace ray_storm
{
  namespace objects
  {
    class Emitting
    {
    public:

      struct RaySample
      {
        random::RandomRay randRay;
        glm::vec3 emittance;
        RaySample() : emittance(glm::vec3(0.0f)) {};
      };

      struct PointSample
      {
        glm::vec3 point;
        glm::vec2 uv;
        PointSample(const glm::vec3 &point, const glm::vec2 &uv) : point(point), uv(uv) {};
      };

      Emitting(const glm::vec3 &emittance)
      {
        this->emittance = emittance;
      }

      virtual ~Emitting() {};

      virtual float getSurfaceArea() = 0;

      virtual PointSample drawRandomSurfacePoint(random::RandomizationHelper &randHelper) = 0;

      virtual void drawRandomRay(random::RandomizationHelper &randHelper, RaySample &raySample) = 0;

      virtual float getPDF(const glm::vec3 &l, const glm::vec3 &n) = 0;

      virtual glm::vec3 getEmittance(const glm::vec3 &l, const glm::vec3 &n, const glm::vec2 &uv)
      {
        return glm::dot(l, n) > 0.0f ? this->emittance : glm::vec3(0.0f);
      }

      virtual bool isEmitting()
      {
        if (glm::any(glm::greaterThan(this->emittance, glm::vec3(0.0f))))
        {
          return true;
        }

        return false;
      }

    private:

      glm::vec3 emittance;
    };
  }
}

#endif