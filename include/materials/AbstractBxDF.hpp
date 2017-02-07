#ifndef ABSTRACT_BXDF_H_
#define ABSTRACT_BXDF_H_

#include "materials/AbstractSVBxDF.h"

namespace ray_storm
{
  namespace materials
  {
    class AbstractBxDF : public AbstractSVBxDF
    {
    public:

      virtual ~AbstractBxDF() {}

      virtual bool delta() const
      {
        return false;
      }
      
    };

    typedef std::shared_ptr<AbstractBxDF> AbstractBxDFPtr;
  }
}

#endif