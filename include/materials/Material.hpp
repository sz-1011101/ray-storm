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

      inline bool evaluateBSDF(
        const glm::vec3 &l,
        const glm::vec3 &n,
        const glm::vec3 &v,
        glm::vec3 &result
      )
      {
        result = glm::vec3(0.0f);
        // decide if we have a refraction or reflection based on the given situation
        const LIGHT_INTERACTION_TYPE type = MaterialHelper::determineType(l, n, v);

        if (!this->checkAvailable(type))
        {
          return false;
        }

        const float reflectivity = this->useFresnel ? this->computeFresnelReflection(-l, n) : this->constReflectance;

        if (type == REFLECTION)
        {
          // to evaluate the brdf, we have to flip the normal around if we reflect at the anti normal side
          glm::vec3 nRefl = dot(n, l) < 0.0f ? -n : n;
          result = reflectivity*this->brdf->evaluate(l, nRefl, v);
        }
        else if (type == REFRACTION)
        {
          // conservation of energy applies here, we also use the intersection normal to flip the IORs later
          result = (1.0f - reflectivity)*this->btdf->evaluate(l, n, v);
        }
        
        return true;
      }

      inline bool sampleBSDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper,
        random::RandomDirection &randDir
      )
      {
        const float reflectivity = this->useFresnel ? this->computeFresnelReflection(in, n) : this->constReflectance;

        // reflect with probabilty proportional to reflectivity
        if (reflectivity > randHelper.drawUniformRandom())
        {
          if (this->brdf == nullptr) // nothing reflects of this since there is no brdf
          {
            return false;
          }
          this->brdf->drawReflectedDirection(in, n, randHelper, randDir);
          return true;
        }
        else if (this->btdf != nullptr) // transmission is happening, if btdf available
        {
          this->btdf->drawRefractedDirection(in, n, randHelper, randDir);
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

        randRay.ray.origin = x + offset(in, n, randDir.direction);
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
        if (!this->checkAvailable(type))
        {
          return false;
        }

        if (type == REFLECTION)
        {
          PDF = this->brdf->getPDF(in, n, out);
          return true;
        }
        else if (type == REFRACTION)
        {
          PDF = this->btdf->getPDF(in, n, out);
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

      inline void setConstReflectance(bool constReflectance)
      {
        this->useFresnel = false;
        this->constReflectance = constReflectance;
      }

    private:

      const float RAY_OFFSET_EPSILON = 0.0001f;

      AbstractBRDFPtr brdf;
      AbstractBTDFPtr btdf;

      float indexOfRefraction;
      bool useFresnel;
      bool constReflectance;

      inline float computeFresnelReflection(const glm::vec3 &in, const glm::vec3 &n)
      {
        // see http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
        // and https://en.wikipedia.org/wiki/Fresnel_equations
        float eta1 = 1.0f;
        float eta2 = this->indexOfRefraction;

        float cosIn = glm::dot(in, n);
        if (cosIn > 0.0f)
        {
          eta1 = this->indexOfRefraction;
          eta2 = 1.0f;
        }

        float sinTr = eta1/eta2*std::sqrt(1.0f - cosIn*cosIn);

        if (sinTr >= 1.0f) // internal reflection
        {
          return 1.0f;
        }
        else // transmission possible
        {
          float cosTr = sqrt(std::max(0.0f, 1.0f - sinTr*sinTr));
          cosIn = std::abs(cosIn);
          // fresnel equations
          const float eta1CosIn = eta1*cosIn;
          const float eta2CosIn = eta2*cosIn;
          const float eta1CosTr = eta1*cosTr;
          const float eta2CosTr = eta2*cosTr;

          float Rs = (eta1CosIn - eta2CosTr)/(eta1CosIn + eta2CosTr);
          float Rp = (eta1CosTr - eta2CosIn)/(eta1CosTr + eta2CosIn);

          return (Rs*Rs + Rp*Rp)*0.5f; // average
        }

      }

    };

    typedef std::shared_ptr<materials::Material> MaterialPtr;
  }
}

#endif