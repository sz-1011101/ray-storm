#ifndef ABSTRACT_DISTANCE_FIELD_H_
#define ABSTRACT_DISTANCE_FIELD_H_

#include <memory>

#include "geometry/Marchable.h"

namespace ray_storm
{
  namespace objects
  {
    class AbstractDistanceField : public Reflector
    {
    public:

      AbstractDistanceField(const materials::MaterialPtr &material) : Reflector(material) {};

      virtual ~AbstractDistanceField() {};
      
    };

    typedef std::shared_ptr<AbstractDistanceField> AbstractDistanceFieldPtr;
  }
}

#endif