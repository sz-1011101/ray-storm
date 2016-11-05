#ifndef BOUNDED_H_
#define BOUNDED_H_

#include "geometry/AxisAlignedBox.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class Bounded
    {
    public:

      const AxisAlignedBox &getBBox() const
      {
        return this->bbox;
      }

    protected:

      AxisAlignedBox bbox;

      
    };
  }
}

#endif