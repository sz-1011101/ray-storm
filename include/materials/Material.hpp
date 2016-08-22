#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "utility/linear_alg.hpp"
#include "materials/AbstractBRDF.h"
#include "materials/AbstractBTDF.h"
#include "geometry/Ray.hpp"
#include "materials/MaterialHelper.hpp"
#include "random/RandomRay.hpp"

namespace ray_storm
{
  namespace materials
  {

    class Material
    {

    public:

      Material(
        AbstractBRDFPtr &brdf,
        AbstractBTDFPtr &btdf,
        float indexOfRefraction = 1.5f
      ) : brdf(brdf), btdf(btdf)
      {
        this->indexOfRefraction = indexOfRefraction;
        this->useFresnel = false;
        this->constReflectance = 1.0f;
      }

      Material(
        AbstractBRDFPtr &brdf,
        float indexOfRefraction = 1.5f
      ) : brdf(brdf), btdf(nullptr)
      {
        this->indexOfRefraction = indexOfRefraction;
        this->useFresnel = false;
        this->constReflectance = 1.0f;
      }

      Material(
        AbstractBTDFPtr &btdf,
        float indexOfRefraction = 1.5f
      ) : brdf(nullptr), btdf(btdf)
      {
        this->indexOfRefraction = indexOfRefraction;
        this->useFresnel = false;
        this->constReflectance = 1.0f;
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
        const glm::vec3 &v
      )
      {
        const float reflectivity = this->computeReflectivity(1.0f, this->indexOfRefraction, -l, n);
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
          return reflectivity*this->brdf->evaluate(l, nRefl, v);
        }
        else if (type == REFRACTION)
        {
          // conservation of energy applies here, we also use the intersection normal to flip the IORs later
          return (1.0f - reflectivity)*this->btdf->evaluate(l, n, v);
        }

        return glm::vec3(0.0f);
      }

      inline bool sampleBSDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper,
        random::RandomDirection &randDir
      )
      {
        const float reflectivity = this->computeReflectivity(1.0f, this->indexOfRefraction, in, n);

        // reflect with probabilty proportional to reflectivity
        if (randHelper.drawUniformRandom() < reflectivity)
        {
          if (this->brdf == nullptr) // nothing reflects of this since there is no brdf
          {
            return false;
          }
          this->brdf->drawReflectedDirection(in, n, randHelper, randDir);
          randDir.PDF *= reflectivity; // fresnel sampling pdf
          return true;
        }
        else if (this->btdf != nullptr) // transmission is happening, if btdf available
        {
          this->btdf->drawRefractedDirection(in, n, randHelper, randDir);
          randDir.PDF *= (1.0f - reflectivity); // fresnel sampling pdf
          return true;
        }

        return false;
      }

      inline bool sampleBSDF(
        const glm::vec3 &in,
        const glm::vec3 &x,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper,
        random::RandomRay &randRay
      )
      {
        random::RandomDirection randDir;
        if (!this->sampleBSDF(in, n, randHelper, randDir))
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
        const glm::vec3 &out,
        float &PDF
      )
      {
        const LIGHT_INTERACTION_TYPE type = MaterialHelper::determineType(out, n, -in);

        const float reflectivity = this->computeReflectivity(1.0f, this->indexOfRefraction, in, n);

        if (!this->checkAvailable(type))
        {
          return false;
        }

        if (type == REFLECTION)
        {
          PDF = reflectivity*this->brdf->getPDF(in, n, out);
          return true;
        }
        else if (type == REFRACTION)
        {
          PDF = (1.0f - reflectivity)*this->btdf->getPDF(in, n, out);
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

      inline void setUseFresnel(bool useFresnel)
      {
        this->useFresnel = useFresnel;
      }

      inline void setConstReflectance(float constReflectance)
      {
        this->useFresnel = false;
        this->constReflectance = constReflectance;
      }

    private:

      const float RAY_OFFSET_EPSILON = 0.001f;

      AbstractBRDFPtr brdf;
      AbstractBTDFPtr btdf;

      float indexOfRefraction;
      bool useFresnel;
      float constReflectance;

      inline float computeReflectivity(float eta1, float eta2, const glm::vec3 &in, const glm::vec3 &n)
      {
        return this->useFresnel ? MaterialHelper::computeFresnelReflection(eta1, eta2, in, n) : this->constReflectance;
      }

    };

    typedef std::shared_ptr<materials::Material> MaterialPtr;
  }
}

#endif