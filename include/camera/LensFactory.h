#ifndef LENS_FACTORY_H_
#define LENS_FACTORY_H_

#include "camera/ApertureLens.hpp"

namespace ray_storm
{
  namespace camera
  {
    class LensFactory
    {
    public:

      static ApertureLensPtr createNPolygon(std::size_t n, float diameter);

    private:

      LensFactory();

    };
  }
}

#endif