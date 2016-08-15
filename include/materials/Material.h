#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "utility/linear_alg.hpp"
#include "materials/AbstractBRDF.h"
#include "materials/AbstractBTDF.h"
#include "geometry/Ray.hpp"

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
        float reflectance = 1.0f,
        float indexOfRefraction = 1.0f,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      );

      Material(
        AbstractBRDFPtr &brdf,
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
        float cosTheta = 0.0f;
        glm::vec3 bsdf(0.0f);
        random::RandomDirection randDir;

        // light emission
        result.emittance = this->emittance;

        // reflecting...
        if (randHelper.drawUniformRandom() < this->reflectance)
        {
          if (!this->reflectAndEvaluateBRDF(v, n, bsdf, randHelper, randDir))
          {
            return false;
          }
          cosTheta = std::max(0.0f, glm::dot(n, randDir.direction));
          result.ray.origin = x + 0.001f*n;
        }
        else // refracting...
        {
          if (this->btdf == nullptr)
          {
            return false;
          }
          // refracting or total inner reflection?
          if (this->btdf->drawRefractedDirection(-v, n, this->indexOfRefraction, randHelper, randDir))
          {
            cosTheta = 1.0f;//std::abs(glm::dot(n, randDir.direction));
            bsdf = this->btdf->evaluate(v, n, randDir.direction);
            result.ray.origin = x + 0.001f*randDir.direction;
          } // try reflecting internally
          else if (this->reflectAndEvaluateBRDF(v, -n, bsdf, randHelper, randDir))
          {
            cosTheta = std::max(0.0f, glm::dot(-n, randDir.direction));
            result.ray.origin = x - 0.001f*n;
          }
          else
          {
            return false;
          }
        }
        result.ray.direction = randDir.direction;
        result.weight = cosTheta*bsdf*randDir.inversePDF;
        return true;
      }


    private:

      AbstractBRDFPtr brdf;
      AbstractBTDFPtr btdf;

      glm::vec3 emittance;

      float reflectance;

      float indexOfRefraction;

      inline bool reflectAndEvaluateBRDF(
        const glm::vec3 &v,
        const glm::vec3 &n,
        glm::vec3 &result,
        random::RandomizationHelper &randHelper,
        random::RandomDirection &randDir
      )
      {
        if (this->brdf == nullptr)
        {
          return false;
        }
        this->brdf->drawReflectedDirection(-v, n, randHelper, randDir);
        result = this->brdf->evaluate(v, n, randDir.direction);
        return true;
      }

    };

    typedef std::shared_ptr<materials::Material> MaterialPtr;
  }
}

#endif