#ifndef DIST_SPHERE_H_
#define DIST_SPHERE_H_

#include "geometry/Marchable.h"
#include "geometry/SpherePrimitive.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class DistSphere : public Marchable
    {
    public:

      DistSphere(const glm::vec3 &position, float radius) : sphere(position, radius)
      {

      }

      float distance(const glm::vec3 &p) const
      {
        return glm::length(p) - this->sphere.getRadius();
      }

      const glm::vec3 &getCenter() const
      {
        return this->sphere.getPosition();
      }

      AxisAlignedBox computeMarchingCube() const
      {
        return sphere.computeBBox();
      }

    private:

      SpherePrimitive sphere;
      
    };
  }
}

#endif