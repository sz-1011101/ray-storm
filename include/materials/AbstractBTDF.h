#ifndef ABSTRACT_BTDF_H_
#define ABSTRACT_BTDF_H_

#include "materials/AbstractSVBxDF.hpp"

namespace ray_storm
{
  namespace materials
  {
    class AbstractBTDF : public AbstractSVBxDF
    {
    public:

      AbstractBTDF(float indexOfRefraction)
      {
        this->indexOfRefraction = indexOfRefraction;
      }

      virtual ~AbstractBTDF() {};

      virtual float getIndexOfRefraction()
      {
        return this->indexOfRefraction;
      }

      virtual void setIndexOfRefraction(float indexOfRefraction)
      {
        this->indexOfRefraction = indexOfRefraction;
      }

    protected:

      float indexOfRefraction;
      
    };

    typedef std::shared_ptr<AbstractBTDF> AbstractBTDFPtr;
  }
}

#endif