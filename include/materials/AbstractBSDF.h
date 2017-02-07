#ifndef ABSTRACT_BSDF_H_
#define ABSTRACT_BSDF_H_

#include "materials/AbstractSVBxDF.h"

namespace ray_storm
{
  namespace materials{

    class AbstractBSDF : public AbstractSVBxDF
    {
    public:

      virtual ~AbstractBSDF() {};
      
    };

    typedef std::shared_ptr<AbstractBSDF> AbstractBSDFPtr;
  }
}

#endif