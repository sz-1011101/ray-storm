#ifndef MATERIAL_INTERACTION_H_
#define MATERIAL_INTERACTION_H_

#include <memory>

#include "materials/Material.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class MaterialInteraction
    {

    public:

      MaterialInteraction(const materials::MaterialPtr &material) : material(material)
      {
        
      }

      virtual ~MaterialInteraction() {};

      materials::Material *getMaterial();

    protected:

      materials::MaterialPtr material;
      
    };
  }
}

#endif