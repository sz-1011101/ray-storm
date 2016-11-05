#ifndef SPHERE_PRIMITIVE_H_
#define SPHERE_PRIMITIVE_H_

#include "utility/common.hpp"
#include "geometry/Intersectable.h"

namespace ray_storm
{
  namespace geometry
  {
    class SpherePrimitive : public Intersectable<SpherePrimitive>
    {
    public:

      SpherePrimitive(const glm::vec3 &position, float radius) : position(position), radius(radius)
      {

      }

      inline bool intersect(const Ray &ray, Intersection<SpherePrimitive> &intersection)
      {
        const float a = glm::dot(ray.direction, ray.direction);
        const glm::vec3 orgMinusCenter = ray.origin - this->position;
        const float b = 2.0f*glm::dot(ray.direction, orgMinusCenter);
        const float c = glm::dot(orgMinusCenter, orgMinusCenter) - this->radius*this->radius;

        const float discriminant = b*b - 4*a*c;

        // ray does not hit the sphere
        if (discriminant < 0.0f) {
          return false;
        }

        // ray hits...
        const float sqrtDiscriminant = std::sqrt(discriminant);
        const float t1 = (-b + sqrtDiscriminant)/(2.0f*a);
        const float t2 = (-b - sqrtDiscriminant)/(2.0f*a);

        // sphere is intersected but behind us
        if (t1 < 0.0f && t2 < 0.0f) {
          return false;
        }

        // sphere is hit
        intersection.t = t1 > 0.0f && t2 > 0.0f ? std::min(t1, t2) : std::max(t1, t2);
        intersection.intersected = this;
        const glm::vec3 iPoint = ray.origin + intersection.t*ray.direction;
        
        const glm::vec3 spherical = utility::Math::cartesianToSpherical(iPoint - position);
        intersection.intersection = SimpleIntersection (iPoint, glm::normalize(iPoint - this->position),
          glm::vec2(spherical.y/(2.0f*M_PI), spherical.z/M_PI));
        return true;
      }

      float getSurfaceArea() const
      {
        return 4.0f*static_cast<float>(M_PI)*this->radius*this->radius;
      }

      const glm::vec3 &getPosition() const
      {
        return this->position;
      }

      float getRadius() const
      {
        return this->radius;
      }

    protected:

      glm::vec3 position;

      float radius;
      
    };
  }
}

#endif