#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <memory>

#include "materials/AbstractSVBxDF.h"

namespace ray_storm
{
  namespace objects
  {
    class Renderable
    {

    public:

      Renderable(const materials::AbstractSVBxDFPtr &material) : material(material)
      {
        
      }

      virtual ~Renderable() {};

      materials::AbstractSVBxDF *getMaterial();

    protected:

      materials::AbstractSVBxDFPtr material;
      
    };
  }
}

#endif