#ifndef DEFAULT_DISTANCE_FIELD_H_
#define DEFAULT_DISTANCE_FIELD_H_

#include "objects/AbstractDistanceField.h"
#include "geometry/RayMarcher.hpp"

namespace ray_storm
{
  namespace objects
  {
    class DefaultDistanceField : public AbstractDistanceField
    {
    public:

      DefaultDistanceField(const geometry::MarchablePtr &marchable, const materials::MaterialPtr &material)
        : AbstractDistanceField(material), marchable(marchable)
      {
        this->bbox = marchable->computeMarchingCube();
      }

      inline bool intersect(const geometry::Ray &ray, geometry::Intersection<Object> &intersection)
      {
        geometry::AxisAlignedBox::BBoxIntersectionInterval bboxIntersect;

        if (!this->bbox.intersect(ray, bboxIntersect))
        {
          return false;
        }

        geometry::SimpleIntersection sInters;
        if (!geometry::RayMarcher::march(ray, this->marchable.get(), sInters))
        {
          return false;
        }

        intersection.intersection = sInters;
        intersection.intersected = this;
        intersection.t = glm::distance(sInters.position, ray.origin);

        return true;
      }

    private:

      geometry::MarchablePtr marchable;
      
    };
  }
}

#endif