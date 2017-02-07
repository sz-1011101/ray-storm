#ifndef COMBINED_BSDF_H_
#define COMBINED_BSDF_H_

#include "materials/AbstractBSDF.h"
#include "materials/AbstractReflectivity.hpp"
#include "materials/MaterialHelper.hpp"

namespace ray_storm
{
  namespace materials
  {
    class CombinedBSDF : public AbstractBSDF
    {
    public:

      CombinedBSDF(
        const AbstractBRDFPtr &brdf,
        const AbstractBTDFPtr &btdf,
        const AbstractReflectivityPtr &reflectivity
      ) : brdf(brdf), btdf(btdf), reflectivity(reflectivity)
      {

      }

      bool checkAvailable(LIGHT_INTERACTION_TYPE type)
      {
        if ((type == REFLECTION && this->brdf == nullptr) || (type == REFRACTION && this->btdf == nullptr)) 
        {
          return false;
        }
        return true;
      }

      glm::vec3 evaluate(
        const SurfaceInteraction &si
      )
      {

        const float refl = this->reflectivity->computeF(1.0f, this->btdf->getIndexOfRefraction(), -si.v, si.n);
        // decide if we have a refraction or reflection based on the given situation
        if (si.type == REFLECTION)
        {
          // to evaluate the brdf, we have to flip the normal around if we reflect at the anti normal side
          SurfaceInteraction siFlippedNormal = si; // FIXME ugly copying -> move to BRDFs?
          siFlippedNormal.n = glm::dot(si.n, si.l) < 0.0f ? -si.n : si.n;
          return refl*this->brdf->evaluate(siFlippedNormal);
        }
        else if (si.type == REFRACTION)
        {
          // conservation of energy applies here, we also use the intersection normal to flip the IORs later
          return (1.0f - refl)*this->btdf->evaluate(si);
        }

        return glm::vec3(0.0f);

      }

      void sample(
        random::RandomizationHelper &randHelper,
        SurfaceInteraction &si
      )
      {
        // fresnel term
        const float rfl = this->reflectivity->computeF(1.0f, this->btdf->getIndexOfRefraction(), si.getIn(), si.n);
        // reflect with probabilty proportional to reflectivity
        if (randHelper.drawUniformRandom() < rfl)
        {
          this->brdf->sample(randHelper, si);
          si.PDF *= rfl; // fresnel sampling pdf
        }
        else // transmission is happening
        {
          this->btdf->sample(randHelper, si);
          si.PDF *= (1.0f - rfl); // fresnel sampling pdf
        }
        si.reflectivity = rfl;
      }

      void pdf(
        SurfaceInteraction &si
      )
      {
        LIGHT_INTERACTION_TYPE type;
        float rfl;
        if (si.sampled) {
          type = si.type;
          rfl = si.reflectivity;
        }
        else
        {
          const glm::vec3 in = si.getIn();
          type = MaterialHelper::determineType(si.getOut(), si.n, -in);
          rfl = this->reflectivity->computeF(1.0f, this->btdf->getIndexOfRefraction(), in, si.n);
        }
        

        if (type == REFLECTION)
        {
          this->brdf->pdf(si);
          si.PDF *= rfl;
        }
        else if (type == REFRACTION)
        {
          this->btdf->pdf(si);
          si.PDF *= (1.0f - rfl);
        }

      }

    private:

      AbstractBRDFPtr brdf;

      AbstractBTDFPtr btdf;

      AbstractReflectivityPtr reflectivity;
      
    };

    typedef std::shared_ptr<CombinedBSDF> CombinedBSDFPtr;
  }
}

#endif