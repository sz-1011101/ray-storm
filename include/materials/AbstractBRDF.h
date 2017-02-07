#ifndef ABSTRACT_BRDF_H_
#define ABSTRACT_BRDF_H_

#include "materials/AbstractBxDF.hpp"

namespace ray_storm
{
  namespace materials
  {
    class AbstractBRDF : public AbstractBxDF
    {
    public:

      virtual ~AbstractBRDF() {}

    };

    typedef std::shared_ptr<AbstractBRDF> AbstractBRDFPtr;

  }
}

#endif