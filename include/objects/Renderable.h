#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <memory>

#include "materials/Material.hpp"

namespace ray_storm
{
  namespace objects
  {
    class Renderable
    {

    public:

      Renderable(const materials::MaterialPtr &material) : material(material)
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