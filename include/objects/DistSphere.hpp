#ifndef DIST_SPHERE_H_
#define DIST_SPHERE_H_

#include "geometry/SpherePrimitive.hpp"
#include "objects/Reflector.h"
#include "geometry/RayMarcher.hpp"

namespace ray_storm
{
  namespace objects
  {
    class DistSphere : public Reflector
    {

    public:

      DistSphere(const glm::vec3 &position, float radius, const materials::MaterialPtr &material)
        : Reflector(material), sphere(position, radius)
      {
        this->bbox = this->sphere.computeBBox();
      }

      inline bool intersect(const geometry::Ray &ray, geometry::Intersection<Object> &intersection)
      {
        geometry::AxisAlignedBox::BBoxIntersectionInterval bboxIntersect;

        if (!this->bbox.intersect(ray, bboxIntersect))
        {
          return false;
        }

        geometry::SimpleIntersection sInters;
        if (!geometry::RayMarcher::march(ray, &this->sphere, sInters))
        {
          return false;
        }

        intersection.intersection = sInters;
        intersection.intersected = this;
        intersection.t = glm::distance(sInters.position, ray.origin);

        return true;
      }

    private:

      geometry::SpherePrimitive sphere;

    };

  }
}

#endif