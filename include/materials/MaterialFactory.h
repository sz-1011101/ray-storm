#ifndef MATERIAL_FACTORY_H_
#define MATERIAL_FACTORY_H_

#include "materials/AbstractSVBxDF.h"
#include "materials/AbstractBRDF.h"
#include "materials/AbstractBTDF.h"
#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace materials
  {
    class MaterialFactory
    {
    private:

      MaterialFactory() {};

    public:

      static AbstractSVBxDFPtr createLambertian(const glm::vec3 &color);

      static AbstractSVBxDFPtr createLambertian(const textures::Abstract2DTexturePtr<glm::vec3> &color);

      static AbstractSVBxDFPtr createMetal(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness
      );

      static AbstractSVBxDFPtr createMetalFresnel(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness,
        float indexOfRefraction,
        float absorption
      );

      static AbstractSVBxDFPtr createMirror(
        const glm::vec3 &color
      );

      static AbstractSVBxDFPtr createMirror(
        const textures::Abstract2DTexturePtr<glm::vec3> &color
      );
    
      static AbstractSVBxDFPtr createGlass(
        const glm::vec3 &color,
        float indexOfRefraction
      );

      static AbstractSVBxDFPtr createShiny(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness
      );

      static AbstractSVBxDFPtr createShiny(
        const textures::Abstract2DTexturePtr<glm::vec3> &diffuse,
        const textures::Abstract2DTexturePtr<glm::vec3> &specular,
        const textures::Abstract2DTexturePtr<float> &shinyness
      );

      static AbstractSVBxDFPtr createShinyFresnel(
        const textures::Abstract2DTexturePtr<glm::vec3> &diffuse,
        const textures::Abstract2DTexturePtr<glm::vec3> &specular,
        const textures::Abstract2DTexturePtr<float> &shinyness,
        float indexOfRefraction
      );

      static AbstractSVBxDFPtr createCombined(
        const AbstractBRDFPtr &brdf,
        const AbstractBTDFPtr &btdf,
        float constReflectance
      );
      
    };
  }
}

#endif