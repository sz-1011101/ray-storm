#ifndef MATERIAL_FACTORY_H_
#define MATERIAL_FACTORY_H_

#include "materials/Material.hpp"

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

      static MaterialPtr createMetal(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float shinyness
      );

      static MaterialPtr createMirror(
        const glm::vec3 &color
      );
    
      static MaterialPtr createGlass(
        const glm::vec3 &color,
        float indexOfRefraction
      );

      static MaterialPtr createDiffuseGlass(
        const glm::vec3 &color,
        float scattering,
        float indexOfRefraction
      );

      static MaterialPtr createSimpleLight(
        const glm::vec3 &emission
      );
      
    };
  }
}

#endif