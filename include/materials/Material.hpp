#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "utility/linear_alg.hpp"
#include "materials/AbstractBRDF.h"
#include "materials/AbstractBTDF.h"
#include "geometry/Ray.hpp"
#include "materials/MaterialHelper.hpp"
#include "random/RandomRay.hpp"
#include "materials/AbstractReflectivity.hpp"
#include "materials/ConstantReflectivity.hpp"

namespace ray_storm
{
  namespace materials
  {

    class Material
    {

    public:

      Material(
        const AbstractBRDFPtr &brdf,
        const AbstractBTDFPtr &btdf,
        const AbstractReflectivityPtr &reflectivity
      ) : brdf(brdf), btdf(btdf), reflectivity(reflectivity)
      {
        if (btdf != nullptr) {
          this->eta = btdf->getIndexOfRefraction();
        }
        else
        {
          this->eta = 1.5f;
        }
      }

      Material(
        const AbstractBRDFPtr &brdf
      ) : brdf(brdf), btdf(nullptr)
      {
        this->reflectivity = AbstractReflectivityPtr(new ConstantReflectivity(1.0f));
        this->eta = 1.0f; // dont care
      }

      Material(
        const AbstractBRDFPtr &brdf,
        float eta,
        const AbstractReflectivityPtr &reflectivity
      ) : brdf(brdf), btdf(nullptr), reflectivity(reflectivity)
      {
        this->eta = eta;
      }

      Material(
        const AbstractBTDFPtr &btdf
      ) : brdf(nullptr), btdf(btdf)
      {
        if (btdf != nullptr) {
          this->eta = btdf->getIndexOfRefraction();
        }
        else
        {
          this->eta = 1.5f;
        }
        this->reflectivity = AbstractReflectivityPtr(new ConstantReflectivity(0.0f));
      }

      inline bool checkAvailable(LIGHT_INTERACTION_TYPE type)
      {
        if ((type == REFLECTION && this->brdf == nullptr) || (type == REFRACTION && this->btdf == nullptr)) 
        {
          return false;
        }
        return true;
      }

      inline glm::vec3 evaluateBSDF(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &v
      )
      {
        if (this->reflectivity == nullptr)
        {
          return glm::vec3(0.0f);
        }
        const float refl = this->reflectivity->computeF(1.0f, this->eta, -v, n);
        // decide if we have a refraction or reflection based on the given situation
        const LIGHT_INTERACTION_TYPE type = MaterialHelper::determineType(l, n, v);

        if (!this->checkAvailable(type))
        {
          return glm::vec3(0.0f);
        }

        if (type == REFLECTION)
        {
          // to evaluate the brdf, we have to flip the normal around if we reflect at the anti normal side
          glm::vec3 nRefl = glm::dot(n, l) < 0.0f ? -n : n;
          return refl*this->brdf->evaluate(l, nRefl, uv, v);
        }
        else if (type == REFRACTION)
        {
          // conservation of energy applies here, we also use the intersection normal to flip the IORs later
          return (1.0f - refl)*this->btdf->evaluate(l, n, uv, v);
        }

        return glm::vec3(0.0f);
      }

      inline bool sampleBSDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper,
        random::RandomDirection &randDir
      )
      {
        if (this->reflectivity == nullptr)
        {
          return false;
        }

        const float rfl = this->reflectivity->computeF(1.0f, this->eta, in, n);

        // reflect with probabilty proportional to reflectivity
        if (randHelper.drawUniformRandom() < rfl)
        {
          if (this->brdf == nullptr) // nothing reflects of this since there is no brdf
          {
            return false;
          }
          this->brdf->drawDirection(in, n, uv, randHelper, randDir);
          randDir.PDF *= rfl; // fresnel sampling pdf
          return true;
        }
        else if (this->btdf != nullptr) // transmission is happening, if btdf available
        {
          this->btdf->drawDirection(in, n, uv, randHelper, randDir);
          randDir.PDF *= (1.0f - rfl); // fresnel sampling pdf
          return true;
        }

        return false;
      }

      inline bool sampleBSDF(
        const glm::vec3 &in,
        const glm::vec3 &x,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper,
        random::RandomRay &randRay
      )
      {
        random::RandomDirection randDir;
        if (!this->sampleBSDF(in, n, uv, randHelper, randDir))
        {
          return false;
        }

        randRay.ray.origin = x + this->offset(in, n, randDir.direction);
        randRay.ray.direction = randDir.direction;
        randRay.PDF = randDir.PDF;

        return true;
      }

      inline bool getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &out,
        float &PDF
      )
      {
        if (this->reflectivity == nullptr)
        {
          return false;
        }

        const LIGHT_INTERACTION_TYPE type = MaterialHelper::determineType(out, n, -in);

        const float rfl = this->reflectivity->computeF(1.0f, this->eta, in, n);

        if (!this->checkAvailable(type))
        {
          return false;
        }

        if (type == REFLECTION)
        {
          PDF = rfl*this->brdf->getPDF(in, n, uv, out);
          return true;
        }
        else if (type == REFRACTION)
        {
          PDF = (1.0f - rfl)*this->btdf->getPDF(in, n, uv, out);
          return true;
        }
        return false;
      }

      inline glm::vec3 offset(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec3 &out
      )
      {
        const LIGHT_INTERACTION_TYPE type = MaterialHelper::determineType(out, n, -in);
        switch (type)
        {
          case REFLECTION:
            return glm::dot(n, in) > 0.0f ? -RAY_OFFSET_EPSILON*n : RAY_OFFSET_EPSILON*n;
          case REFRACTION:
            return glm::dot(n, in) > 0.0f ? RAY_OFFSET_EPSILON*n : -RAY_OFFSET_EPSILON*n;
          default:
            return glm::vec3(0.0f);
        }
      }

      inline AbstractBRDFPtr getBRDF()
      {
        return this->brdf;
      }

      inline AbstractBTDFPtr getBTDF()
      {
        return this->btdf;
      }

      inline void setEta(float eta)
      {
        this->eta = eta;
        if (this->btdf != nullptr) {
          this->btdf->setIndexOfRefraction(eta);
        }
      }

    private:

      const float RAY_OFFSET_EPSILON = 0.001f;

      AbstractBRDFPtr brdf;
      AbstractBTDFPtr btdf;

      float eta;
      AbstractReflectivityPtr reflectivity;

    };

    typedef std::shared_ptr<materials::Material> MaterialPtr;
  }
}

#endif