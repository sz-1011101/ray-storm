#ifndef AXIS_ALIGNED_BOX_H_
#define AXIS_ALIGNED_BOX_H_

#include <vector>

#include "geometry/Intersectable.h"

namespace ray_storm
{
  namespace geometry
  {
    class AxisAlignedBox : public Intersectable<AxisAlignedBox>
    {

    private:

      enum BOX_SIDE
      {
        X_PLANE = 0, Y_PLANE, Z_PLANE
      };

      glm::vec3 mapNormal(BOX_SIDE side)
      {
        switch (side)
        {
          case X_PLANE:
           return glm::vec3(1, 0, 0);
          case Y_PLANE:
           return glm::vec3(0, 1, 0);
          case Z_PLANE:
           return glm::vec3(0, 0, 1);
        }
        return glm::vec3(0.0f);
      }

      bool initialized;

    public:

      AxisAlignedBox()
      {
        this->origin = glm::vec3(0.0f);
        this->upperBounds = glm::vec3(0.0f);
        this->initialized = false;
      }

      AxisAlignedBox(const glm::vec3 &origin, const glm::vec3 &extends)
      {
        this->origin = origin;
        this->upperBounds = this->origin + extends;
        this->initialized = true;
      }

      const glm::vec3 &getOrigin() const
      {
        return this->origin;
      }

      const glm::vec3 &getUpperBounds() const
      {
        return this->upperBounds;
      }

      glm::vec3 computeCenter() const
      {
        return this->origin + (this->upperBounds - this->origin)/2.0f;
      }

      void cover(const glm::vec3 &point)
      {
        if (!initialized)
        {
          // some paranoia epsilon
          this->origin = point - glm::vec3(0.01);
          this->upperBounds = point + glm::vec3(0.01);
          initialized = true;
        }
        else
        {
          this->origin = glm::min(this->origin, point - glm::vec3(0.01));
          this->upperBounds = glm::max(this->upperBounds, point + glm::vec3(0.01));
        }
      }

      void cover(const AxisAlignedBox &box)
      {
        this->cover(box.getOrigin());
        this->cover(box.getUpperBounds());
      }
      
      bool intersect(const Ray &ray, Intersection<AxisAlignedBox> &intersection)
      {
        // Sample code from http://www.cs.utah.edu/~awilliam/box/box.pdf
        // TODO optimize and remove div-by-0 problem
        float tmin, tmax, tymin, tymax, tzmin, tzmax;
        const glm::vec3 &ro = ray.origin;
        const glm::vec3 &rd = ray.direction;
        BOX_SIDE side = X_PLANE;

        float t1 = (this->origin.x - ro.x) / rd.x;
        float t2 = (this->upperBounds.x - ro.x) / rd.x;

        if (rd.x >= 0.0f)
        {
          tmin = t1;
          tmax = t2;
        }
        else
        {
          tmin = t2;
          tmax = t1;
        }

        t1 = (this->origin.y - ro.y) / rd.y;
        t2 = (this->upperBounds.y - ro.y) / rd.y;

        if (rd.y >= 0.0f)
        {
          tymin = t1;
          tymax = t2;
        }
        else
        {
          tymin = t2;
          tymax = t1;
        }

        if ((tmin > tymax) || (tymin > tmax))
        {
          return false;
        }

        if (tymin > tmin)
        {
          tmin = tymin;
          side = Y_PLANE;
        }

        tmax = std::min(tymax, tmax);

        t1 = (this->origin.z - ro.z) / rd.z;
        t2 = (this->upperBounds.z - ro.z) / rd.z;

        if (rd.z >= 0.0f)
        {
          tzmin = t1;
          tzmax = t2;
        }
        else
        {
          tzmin = t2;
          tzmax = t1;
        }

        if ((tmin > tzmax) || (tzmin > tmax))
        {
          return false;
        }
        
        if (tmin < tzmin)
        {
          tmin = tzmin;
          side = Z_PLANE;
        }
        tmax = std::min(tmax, tzmax);

        glm::vec3 iNormal;

        if (tmin >= 0.0f) // box is in front of ray
        {
          intersection.t = tmin;
          iNormal = this->mapNormal(side);
          if (glm::dot(iNormal, rd) > 0.0f)
          {
            iNormal = -iNormal;
          }
        }
        else if (tmax <= 0.0f) // box is behind ray
        {
          return false;
        }
        else // we are inside the box
        {
          intersection.t = tmax;
          iNormal = this->mapNormal(side);
          if (glm::dot(iNormal, rd) < 0.0f)
          {
            iNormal = -iNormal;
          }
        }
        
        const glm::vec3 iPoint = ro + intersection.t*rd;

        // TODO texture coordinate
        intersection.intersection = SimpleIntersection (iPoint, iNormal, glm::vec2(0.0f));
        intersection.intersected = this;

        return true;

      }

    private:

      glm::vec3 origin;
      glm::vec3 upperBounds;
      
    };
  }
}

#endif