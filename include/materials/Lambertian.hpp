#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "utility/common.hpp"
#include "materials/AbstractBRDF.h"
#include "textures/Abstract2DTexture.h"
#include "textures/TextureFactory.h"

namespace ray_storm
{
  namespace materials
  {
    
    class Lambertian : public AbstractBRDF
    {
    public:

      Lambertian(const glm::vec3 &albedo)
      {
        this->albedo = textures::TextureFactory::createConstant2DTexture(albedo);
      }

      Lambertian(const textures::Abstract2DTexturePtr<glm::vec3> &albedo)
      {
        this->albedo = albedo;
      }

      glm::vec3 evaluate(
        const SurfaceInteraction &si
      )
      {
        return this->albedo->sample(si.uv)/static_cast<float>(M_PI);
      }

      void sample(
        random::RandomizationHelper &randHelper,
        SurfaceInteraction &si
      )
      {
        si.setOut(randHelper.drawCosineWeightedRandomHemisphereDirection(si.n, 1.0f));
        this->pdf(si);
        si.type = REFLECTION;
        si.reflectivity = 1.0f;
        si.finalizeSampling();
      }

      void pdf(
        SurfaceInteraction &si
      )
      {
        si.PDF = random::RandomizationHelper::cosineRandomHemispherePDF(dot(si.n, si.getOut()), 1.0f);
        si.delta = false;
      }

    private:

      textures::Abstract2DTexturePtr<glm::vec3> albedo;
      
    };
  }
}

#endif