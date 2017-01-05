#ifndef BOX_H_
#define BOX_H_

#include "objects/Reflector.h"
#include "geometry/AxisAlignedBox.hpp"

namespace ray_storm
{
  namespace objects
  {
    class Box : public Reflector
    {
    public:
      
      Box(const glm::vec3 &origin, const glm::vec3 &extends,
        const materials::MaterialPtr &material
      ) : Reflector(material), aaBox(origin, extends)
      {
        this->origin = origin;
        this->extends = extends;
        this->bbox = this->aaBox;
      }

      inline bool intersect(const geometry::Ray &ray, geometry::Intersection<Object> &intersection) {
        geometry::Intersection<geometry::AxisAlignedBox> aaIntersect;

        if (!this->aaBox.intersect(ray, aaIntersect))
        {
          return false;
        }

        intersection.intersection = aaIntersect.intersection;
        intersection.intersected = this;
        intersection.t = aaIntersect.t;
        this->modifyNormal(ray.direction, intersection.intersection);
        return true;
      }

    private:

      // the actual box
      geometry::AxisAlignedBox aaBox;
      glm::vec3 origin;
      glm::vec3 extends;
      
    };
  }
}

#endif