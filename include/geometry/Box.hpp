#ifndef BOX_H_
#define BOX_H_

#include "geometry/Reflector.h"
#include "geometry/AxisAlignedBox.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class Box : public Reflector
    {
    public:
      
      Box(const glm::vec3 &origin, const glm::vec3 &extends,
        materials::MaterialPtr &material
      ) : Reflector(material), aaBox(origin, extends)
      {
        this->origin = origin;
        this->extends = extends;
        this->bbox = this->aaBox;
      }

      inline bool intersect(const Ray &ray, Intersection<Object> &intersection) {
        Intersection<AxisAlignedBox> aaIntersect;

        if (!this->aaBox.intersect(ray, aaIntersect))
        {
          return false;
        }

        intersection.intersection = aaIntersect.intersection;
        intersection.intersected = this;
        intersection.t = aaIntersect.t;

        return true;
      }

    private:

      // the actual box
      AxisAlignedBox aaBox;
      glm::vec3 origin;
      glm::vec3 extends;
      
    };
  }
}

#endif