#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <memory>

#include "materials/Material.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class Renderable
    {

    public:

      Renderable(materials::MaterialPtr &material) : material(material)
      {
        
      }

      virtual ~Renderable() {};

      materials::Material *getMaterial();

    protected:

      materials::MaterialPtr material;
      
    };
  }
}

#endif