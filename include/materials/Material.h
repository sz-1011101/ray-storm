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

      struct LightInteraction
      {
        // next ray direction
        geometry::Ray ray;
        // weight of that direction
        glm::vec3 weight;
        // emission from this material
        glm::vec3 emittance;
      };

      Material(
        AbstractBRDFPtr &brdf,
        AbstractBTDFPtr &btdf,
        float indexOfRefraction,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

      Material(
        AbstractBRDFPtr &brdf,
        float indexOfRefraction,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

      Material(
        AbstractBTDFPtr &btdf,
        float indexOfRefraction,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

      inline bool computeLightInteraction(
        const glm::vec3 &v,
        const glm::vec3 &x,
        const glm::vec3 &n,
        random::RandomizationHelper &randHelper,
        LightInteraction &result
      )
      {
        glm::vec3 bsdf(0.0f);
        random::RandomDirection randDir;

        // light emission
        result.emittance = this->emittance;
        // reflecting...
        if (randHelper.drawUniformRandom() < this->computeFresnelReflection(-v, n))
        {
          if (this->brdf == nullptr)
          {
            return false;
          }

          this->brdf->drawReflectedDirection(-v, n, randHelper, randDir);
          result.ray.origin = x + 0.001f*n;
          bsdf = this->brdf->evaluate(randDir.direction, n, v);
        }
        else // refracting...
        {
          if (this->btdf == nullptr)
          {
            return false;
          }
          glm::vec3 idealRefractionV;
          MaterialHelper::refract(1.0f, this->indexOfRefraction, -v, n, idealRefractionV);

          this->btdf->drawRefractedDirection(-v, n, idealRefractionV, randHelper, randDir);
          result.ray.origin = x + 0.001f*randDir.direction;
          glm::vec3 idealRefractionL;
          MaterialHelper::refract(this->indexOfRefraction, 1.0f, -randDir.direction, -n, idealRefractionL);
          bsdf = this->btdf->evaluate(randDir.direction, n, v, idealRefractionL);
        }

        result.ray.direction = randDir.direction;
        result.weight = bsdf*randDir.inversePDF;
        return true;
      }


    private:

      AbstractBRDFPtr brdf;
      AbstractBTDFPtr btdf;

      glm::vec3 emittance;

      float indexOfRefraction;

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