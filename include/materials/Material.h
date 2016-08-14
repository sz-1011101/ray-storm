#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "utility/linear_alg.hpp"
#include "materials/AbstractBRDF.h"

namespace ray_storm
{
  namespace materials
  {

    class Material
    {

    public:

      Material(AbstractBRDFPtr &brdf, const glm::vec3 &emittance = glm::vec3(0.0f));

      glm::vec3 evaluateBRDF(const glm::vec3 &l, 
        const glm::vec3 &n, const glm::vec3 &v);

      void drawReflectedRay(const glm::vec3 &in, const glm::vec3 &position, const glm::vec3 &n, 
        random::RandomizationHelper &randHelper, random::RandomRay &randRay);

      glm::vec3 getEmittance();

      float getFresnelReflectedRatio();

      float getFresnelRefractedRatio();

    private:

      AbstractBRDFPtr brdf;

      glm::vec3 emittance;

      float fresnelReflectedRatio;

      float fresnelRefractedRatio;

    };

    typedef std::shared_ptr<materials::Material> MaterialPtr;
  }
}

#endif