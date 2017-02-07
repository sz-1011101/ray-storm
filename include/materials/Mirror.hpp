#ifndef MIRROR_H_
#define MIRROR_H_

#include "materials/AbstractBRDF.h"
#include "textures/Abstract2DTexture.h"
#include "textures/TextureFactory.h"

namespace ray_storm
{
  namespace materials
  {
    class Mirror : public AbstractBRDF
    {
    public:

      Mirror(const glm::vec3 &reflectance)
      {
        this->reflectance = textures::TextureFactory::createConstant2DTexture(reflectance);
      }

      Mirror(const textures::Abstract2DTexturePtr<glm::vec3> &reflectance)
      {
        this->reflectance = reflectance;
      }
      
      glm::vec3 evaluate(
        const SurfaceInteraction &si
      )
      {
        return si.reflectivity*this->reflectance->sample(si.uv);
      }

      void sample(
        random::RandomizationHelper &randHelper,
        SurfaceInteraction &si
      )
      {
        si.setOut(glm::normalize(glm::reflect(si.getIn(), si.n)));
        this->pdf(si);
        si.type = REFLECTION;
        si.reflectivity = 1.0f;
        si.finalizeSampling();
      }

      void pdf(
        SurfaceInteraction &si
      )
      {
        si.PDF = 1.0f;
        si.delta = true;
      }

      bool delta() const
      {
        return true;
      }

    private:

      textures::Abstract2DTexturePtr<glm::vec3> reflectance;
      
    };
  }
}

#endif