#ifndef BOX_H_
#define BOX_H_

#include "geometry/AxisAlignedBox.hpp"
#include "geometry/Object.h"

namespace ray_storm
{
  namespace geometry
  {
    class Box : public Object
    {
    public:
      
      Box(const glm::vec3 &origin, const glm::vec3 &extends,
        materials::MaterialPtr &material
      ) : Object(material, glm::vec3(0.0f)), aaBox(origin, extends)
      {
        this->origin = origin;
        this->extends = extends;
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

      float getSurfaceArea()
      {
        return 2.0f*(extends.x*extends.y + extends.x*extends.z + extends.z*extends.y);
      }

      glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper)
      {
        // TODO implement
        return this->origin;
      }

      float getPDF()
      {
        // TODO implement
        return 0.0f;
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