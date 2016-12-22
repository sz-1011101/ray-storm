#ifndef PHONG_H_
#define PHONG_H_

#include "materials/AbstractBRDF.h"

namespace ray_storm
{
  namespace materials
  {
    class Phong : public AbstractBRDF
    {

    public:

      Phong(const glm::vec3 &kD, const glm::vec3 &kS, float e)
      {
        // the famous three phong parameters
        this->kD = textures::TextureFactory::createConstant2DTexture<glm::vec3>(kD);
        this->kS = textures::TextureFactory::createConstant2DTexture<glm::vec3>(kS);
        this->e = textures::TextureFactory::createConstant2DTexture<float>(e);
      }

      Phong(
        const textures::Abstract2DTexturePtr<glm::vec3> &kD,
        const textures::Abstract2DTexturePtr<glm::vec3> &kS,
        const textures::Abstract2DTexturePtr<float> &e
      )
      {
        // the famous three phong parameters
        this->kD = kD;
        this->kS = kS;
        this->e = e;
      }

      glm::vec3 evaluate(
        const glm::vec3 &l, 
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &v
      )
      {
        const glm::vec3 r = glm::normalize(glm::reflect(-l, n)); // ideal reflection of light
        const float e_uv = this->e->sample(uv);
        return this->kD->sample(uv)/static_cast<float>(M_PI) + 
          (this->kS->sample(uv)*(e_uv + 2.0f)/(2.0f*static_cast<float>(M_PI)))*std::pow(std::max(0.0f, dot(r, v)), e_uv);
      }

      void drawDirection(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        random::RandomizationHelper &randHelper, 
        random::RandomDirection &randDir)
      {

        glm::vec3 r = glm::normalize(glm::reflect(in, n));
        const float e_uv = this->e->sample(uv);
        randDir.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(r, e_uv);
        randDir.PDF = this->getPDF(in, n, uv, randDir.direction);
      }

      float getPDF(
        const glm::vec3 &in,
        const glm::vec3 &n,
        const glm::vec2 &uv,
        const glm::vec3 &out
      )
      {
        glm::vec3 r = glm::normalize(glm::reflect(in, n));
        const float e_uv = this->e->sample(uv);
        return random::RandomizationHelper::cosineRandomHemispherePDF(glm::dot(r, out), e_uv);
      }

    private:

      textures::Abstract2DTexturePtr<glm::vec3> kD;
      textures::Abstract2DTexturePtr<glm::vec3> kS;
      textures::Abstract2DTexturePtr<float> e;

      float samplingExponent;
      float specProb;
      float diffProb;
      
    };
  }
}

#endif