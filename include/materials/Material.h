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
          if (this->brdf == nullptr)
          {
            return false;
          }

          this->brdf->drawReflectedDirection(-v, n, randHelper, randDir);
          result.ray.origin = x + 0.001f*n;
          bsdf = this->brdf->evaluate(randDir.direction, n, v);
          cosTheta = std::max(0.0f, glm::dot(n, randDir.direction));
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
          cosTheta = 1.0f;//std::abs(glm::dot(n, randDir.direction));
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

    };

    typedef std::shared_ptr<materials::Material> MaterialPtr;
  }
}

#endif