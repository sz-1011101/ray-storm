#ifndef ABSTRACT_BRDF_H_
#define ABSTRACT_BRDF_H_

#include "materials/AbstractSVBxDF.hpp"

namespace ray_storm
{
  namespace materials
  {
    class AbstractBRDF : public AbstractSVBxDF
    {
    public:

      virtual ~AbstractBRDF() {};

    };

    typedef std::shared_ptr<AbstractBRDF> AbstractBRDFPtr;

  }
}

#endif