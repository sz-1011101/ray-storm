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
        const SurfaceInteraction &si
      )
      {
        const glm::vec3 r = glm::normalize(glm::reflect(-si.l, si.n)); // ideal reflection of light
        const float e_uv = this->e->sample(si.uv);
        return this->kD->sample(si.uv)/static_cast<float>(M_PI) + 
          (si.reflectivity*this->kS->sample(si.uv)*(e_uv + 2.0f)/(2.0f*static_cast<float>(M_PI)))*std::pow(std::max(0.0f, dot(r, si.v)), e_uv);
      }

      void sample(
        random::RandomizationHelper &randHelper,
        SurfaceInteraction &si
      )
      {
        glm::vec3 dir;
        if (randHelper.drawUniformRandom() < this->computeCosineSamplingProbability(si.uv))
        {
          const glm::vec3 r = glm::normalize(glm::reflect(si.getIn(), si.n));
          const float e_uv = this->e->sample(si.uv);
          dir = randHelper.drawCosineWeightedRandomHemisphereDirection(r, e_uv);
        }
        else
        {
          dir = randHelper.drawCosineWeightedRandomHemisphereDirection(si.n, 1.0f);
        }
        si.setOut(dir);
        this->pdf(si);
        si.type = REFLECTION;
        si.reflectivity = 1.0f;
        si.finalizeSampling();
      }

      void pdf(
        SurfaceInteraction &si
      )
      {
        const glm::vec3 in = si.getIn();
        const glm::vec3 out = si.getOut();
        glm::vec3 r = glm::normalize(glm::reflect(in, si.n));
        const float e_uv = this->e->sample(si.uv);
        const float cosProb = this->computeCosineSamplingProbability(si.uv);
        si.PDF = cosProb*random::RandomizationHelper::cosineRandomHemispherePDF(glm::dot(r, out), e_uv) + 
          (1.0f - cosProb)*random::RandomizationHelper::cosineRandomHemispherePDF(glm::dot(si.n, out), 1.0f);
        si.delta = false;
      }

    private:

      textures::Abstract2DTexturePtr<glm::vec3> kD;
      textures::Abstract2DTexturePtr<glm::vec3> kS;
      textures::Abstract2DTexturePtr<float> e;

      float samplingExponent;

      /**
       * @brief      Calculates the cosine sampling probability, that is
       *             the probability to sample the specular part of the brdf.
       *
       * @param[in]  uv    Current texture coordinates.
       *
       * @return     The cosine sampling probability.
       */
      float computeCosineSamplingProbability(const glm::vec2 &uv)
      {
        const float e_uv = this->e->sample(uv);
        const glm::vec3 kS_uv = this->kS->sample(uv);
        const glm::vec3 kD_uv = this->kD->sample(uv);

        const float specular = glm::length(kS_uv)*std::max(1.0f, e_uv*0.025f);
        const float diffuse = glm::length(kD_uv);

        return specular/(specular + diffuse);
      }
      
    };
  }
}

#endif