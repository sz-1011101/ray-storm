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
        const glm::vec3 &l, 
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &v
      )
      {
        return this->albedo->sample(uv)/static_cast<float>(M_PI);
      }

      void drawDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir
      )
      {
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(n, 1.0f);
        randDir.PDF = this->getPDF(in, n, uv, randDir.direction);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &out
      )
      {
        return random::RandomizationHelper::cosineRandomHemispherePDF(dot(n, out), 1.0f);
      }

    private:

      textures::Abstract2DTexturePtr<glm::vec3> albedo;
      
    };
  }
}

#endif