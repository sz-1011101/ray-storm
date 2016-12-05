#ifndef DIST_SPHERE_H_
#define DIST_SPHERE_H_

#include "objects/Reflector.h"
#include "geometry/Marchable.h"
#include "geometry/RayMarcher.hpp"

namespace ray_storm
{
  namespace objects
  {
    class DistSphere : public Reflector, public geometry::Marchable
    {

    public:

      DistSphere(const glm::vec3 &position, float radius, const materials::MaterialPtr &material)
        : Reflector(material), position(position), radius(radius)
      {
        this->bbox = geometry::AxisAlignedBox(position - glm::vec3(radius), glm::vec3(2.0f*radius));
      }

      virtual float distance(const glm::vec3 &p) const
      {
        return glm::length(p) - this->radius;
      }

      virtual const glm::vec3 &getCenter() const
      {
        return this->position;
      }

      inline bool intersect(const geometry::Ray &ray, geometry::Intersection<Object> &intersection)
      {
        geometry::AxisAlignedBox::BBoxIntersectionInterval bboxIntersect;

        if (!this->bbox.intersect(ray, bboxIntersect))
        {
          return false;
        }
        geometry::SimpleIntersection sInters;
        if (!geometry::RayMarcher::march(ray, this, sInters))
        {
          return false;
        }

        intersection.intersection = sInters;
        intersection.intersected = this;
        intersection.t = glm::distance(sInters.position, ray.origin);

        return true;
      }

    private:

      glm::vec3 position;

      float radius;

    };

  }
}

#endif