#ifndef REFLECTIVITY_BRDF_H_
#define REFLECTIVITY_BRDF_H_

#include "materials/AbstractBRDF.h"
#include "materials/AbstractReflectivity.h"

namespace ray_storm
{
  namespace materials
  {
    class ReflectivityBRDF : public AbstractBRDF
    {
    public:
      
      ReflectivityBRDF(
        const AbstractBRDFPtr &brdf,
        const AbstractReflectivityPtr &reflectivity
      ) : brdf(brdf), reflectivity(reflectivity)
      {

      }

      glm::vec3 evaluate(
        const SurfaceInteraction &si
      )
      {
        SurfaceInteraction siMod = si;
        siMod.reflectivity = si.sampled ? si.reflectivity : this->reflectivity->computeF(1.0f, -si.v, si.n);
        return this->brdf->evaluate(siMod);
      }

      void sample(
        random::RandomizationHelper &randHelper,
        SurfaceInteraction &si
      )
      {
        this->brdf->sample(randHelper, si);
        si.reflectivity = this->reflectivity->computeF(1.0f, si.getIn(), si.n);
      }

      void pdf(
        SurfaceInteraction &si
      )
      {
        this->brdf->pdf(si);
      }
    
    private:

      AbstractBRDFPtr brdf;

      AbstractReflectivityPtr reflectivity;

    };
  }
}

#endif