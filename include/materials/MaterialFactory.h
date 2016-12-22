#ifndef MATERIAL_FACTORY_H_
#define MATERIAL_FACTORY_H_

#include "materials/Material.hpp"
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

      static MaterialPtr createLambertian(const glm::vec3 &color);

      static MaterialPtr createLambertian(const textures::Abstract2DTexturePtr<glm::vec3> &color);

      static MaterialPtr createMetal(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness
      );

      static MaterialPtr createMetalFresnel(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness,
        float indexOfRefraction,
        float absorption
      );

      static MaterialPtr createMirror(
        const glm::vec3 &color
      );

      static MaterialPtr createMirror(
        const textures::Abstract2DTexturePtr<glm::vec3> &color
      );
    
      static MaterialPtr createGlass(
        const glm::vec3 &color,
        float indexOfRefraction
      );

      static MaterialPtr createDiffuseGlass(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float scattering, 
        float indexOfRefraction
      );

      static MaterialPtr createShiny(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness,
        float indexOfRefraction
      );

      static MaterialPtr createShiny(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness
      );

      static MaterialPtr createCombined(
        const AbstractBRDFPtr &brdf,
        const AbstractBTDFPtr &btdf,
        float constReflectance
      );
      
    };
  }
}

#endif