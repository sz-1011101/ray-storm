#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <memory>

#include "materials/AbstractMaterial.h"

namespace ray_storm
{
  namespace geometry
  {
    class Renderable
    {

    public:

      Renderable(materials::AbstractMaterialPtr &material) : material(material)
      {

      }

      virtual ~Renderable() {};

      materials::AbstractMaterial *getMaterial();

    protected:

      materials::AbstractMaterialPtr material;
      
    };
  }
}

#endif