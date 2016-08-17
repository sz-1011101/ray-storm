#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "utility/linear_alg.hpp"
#include "materials/AbstractBRDF.h"
#include "materials/AbstractBTDF.h"
#include "geometry/Ray.hpp"
#include "materials/MaterialHelper.hpp"

namespace ray_storm
{
  namespace materials
  {

    class Material
    {

    public:

      enum LIGHT_INTERACTION_TYPE
      {
        REFLECTION = 0,
        REFRACTION
      };

      struct LightInteraction
      {
        // next ray direction
        geometry::Ray ray;
        // weight of that direction
        glm::vec3 bsdf;
      };

      Material(
        AbstractBRDFPtr &brdf,
        AbstractBTDFPtr &btdf,
        float indexOfRefraction = 1.5f,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      ) : brdf(brdf), btdf(btdf)
      {
        this->indexOfRefraction = indexOfRefraction;
        this->emittance = emittance;
        this->useFresnel = false;
        this->constReflectance = 1.0f;
      }

      Material(
        AbstractBRDFPtr &brdf,
        float indexOfRefraction = 1.5f,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      ) : brdf(brdf), btdf(nullptr)
      {
        this->indexOfRefraction = indexOfRefraction;
        this->emittance = emittance;
        this->useFresnel = false;
        this->constReflectance = 1.0f;
      }

      Material(
        AbstractBTDFPtr &btdf,
        float indexOfRefraction = 1.5f,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      ) : brdf(nullptr), btdf(btdf)
      {
        this->indexOfRefraction = indexOfRefraction;
        this->emittance = emittance;
        this->useFresnel = false;
        this->constReflectance = 1.0f;
      }

      inline bool computeLightDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper,
        random::RandomDirection &randDir,
        LIGHT_INTERACTION_TYPE &type
      )
      {
        const float reflectFresnel = this->useFresnel ? this->computeFresnelReflection(in, n) : this->constReflectance;
        // reflecting...
        if (randHelper.drawUniformRandom() < reflectFresnel)
        {
          if (this->brdf == nullptr)
          {
            return false;
          }

          this->brdf->drawReflectedDirection(in, n, randHelper, randDir);
          type = REFLECTION;
        }
        else // refracting...
        {
          if (this->btdf == nullptr)
          {
            return false;
          }
          if (this->btdf->drawRefractedDirection(in, n, randHelper, randDir))
          {
            type = REFRACTION;
          }
          else
          {
            type = REFLECTION;
          }
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
        LIGHT_INTERACTION_TYPE type = glm::dot(l, n) >= 0.0f ? REFLECTION : REFRACTION;

        if ((type == REFLECTION && this->brdf == nullptr) || (type == REFRACTION && this->btdf == nullptr)) 
        {
          return false;
        }

        const float reflectFresnel = this->useFresnel ? this->computeFresnelReflection(-l, n) : this->constReflectance;

        if (type == REFLECTION)
        {
          result = reflectFresnel*this->brdf->evaluate(l, n, v);
        }
        else if (type == REFRACTION)
        {
          result = (1.0f - reflectFresnel)*this->btdf->evaluate(l, n, v);
        }
        
        return true;
      }

      inline bool computeLightInteraction(
        const glm::vec3 &in,
        const glm::vec3 &x,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper,
        LightInteraction &interaction
      )
      {
        random::RandomDirection randDir;
        LIGHT_INTERACTION_TYPE type;

        if (!this->computeLightDirection(in, n, randHelper, randDir, type))
        {
          return false;
        }

        const float cosTheta = glm::dot(in, n);
        const glm::vec3 nRef = (cosTheta > 0.0f) ? -n : n;
        const int offset = type == REFRACTION ? -1 : 1; 
        if (!this->evaluateBSDF(randDir.direction, nRef, -in, interaction.bsdf))
        {
          return false;
        }

        interaction.ray = geometry::Ray(x + offset*RAY_OFFSET_EPSILON*nRef, randDir.direction);
        interaction.bsdf *= randDir.inversePDF; // inverse sampling pdf!

        return true;
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

      inline glm::vec3 getEmittance()
      {
        return this->emittance;
      }


    private:

      const float RAY_OFFSET_EPSILON = 0.00001f;

      AbstractBRDFPtr brdf;
      AbstractBTDFPtr btdf;

      glm::vec3 emittance;

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