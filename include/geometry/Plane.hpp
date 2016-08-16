#ifndef PLANE_H_
#define PLANE_H_

#include "geometry/Intersectable.h"

namespace ray_storm
{
  namespace geometry
  {

    class Plane : public Intersectable<Plane>
    {
    public:

      Plane(const glm::vec3 &point, const glm::vec3 &normal)
      {
        this->point = point;
        this->normal = glm::normalize(normal);
      }

      inline bool intersect(const Ray &ray, Intersection<Plane> &intersection)
      {

        const float normalDotDirection = glm::dot(this->normal, ray.direction);

        if (normalDotDirection == 0.0f)
        {
          return false;
        }

        const float t = glm::dot(this->normal, this->point - ray.origin)/normalDotDirection;

        // behind ray origin
        if (t < 0.0f)
        {
          return false;
        }

        // normal on both sides!
        const glm::vec3 fNormal = normalDotDirection < 0.0f ? this->normal : -this->normal;
        intersection.intersected = this;
        intersection.intersection = SimpleIntersection(ray.origin + t*ray.direction, fNormal);
        intersection.t = t;

        return true;
      }

    private:

      glm::vec3 point;

      glm::vec3 normal;

      
    };

  }
}

#endif