#ifndef GLASS_H_
#define GLASS_H_

#include "materials/AbstractBTDF.h"
#include "materials/MaterialHelper.hpp"

namespace ray_storm
{
  namespace materials
  {
    class Glass : public AbstractBTDF
    {
    public:

      Glass(
        const glm::vec3 &color,
        float indexOfRefraction = 1.5f
      ) : AbstractBTDF(indexOfRefraction)
      {
        this->color = color;
      }

      glm::vec3 evaluate(
        const SurfaceInteraction &si
      )
      {
        return (1.0f - si.reflectivity)*this->color;
      }

      virtual void sample(
        random::RandomizationHelper &randHelper,
        SurfaceInteraction &si
      )
      {
        glm::vec3 nRef;
        glm::vec3 refract;
        MaterialHelper::refract(1.0f, this->indexOfRefraction, si.getIn(), si.n, refract, nRef);
        si.setOut(refract);
        si.type = REFRACTION;
        si.reflectivity = 0.0f;
        this->pdf(si);
        si.finalizeSampling();
      }

      void pdf(
        SurfaceInteraction &si
      )
      {
        si.PDF = 1.0f;
        si.delta = true;
      }

      bool delta() const
      {
        return true;
      }

    private:

      glm::vec3 color;
      
    };
  }
}

#endif