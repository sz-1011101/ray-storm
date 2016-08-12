#ifndef SPHERE_H_
#define SPHERE_H_

#include "geometry/Object.h"

namespace ray_storm
{
  namespace geometry
  {
    
    class Sphere : public Object
    {

    public:

      Sphere(const glm::vec3 &position, float radius, materials::AbstractMaterialPtr &material) : Object(material) {
        this->position = position;
        this->radius = radius;
      }

      inline bool intersect(const Ray &ray, Intersection<Object> &intersection) {
        const float a = glm::dot(ray.direction, ray.direction);
        const glm::vec3 orgMinusDir = ray.origin - ray.direction;
        const float b = 2.0f*glm::dot(ray.direction, orgMinusDir);
        const float c = glm::dot(orgMinusDir, orgMinusDir) - this->radius*this->radius;

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

        SimpleIntersection si(iPoint, glm::normalize(iPoint - this->position));
        intersection.intersection = si;
        return true;
      }

    protected:

      glm::vec3 position;

      float radius;

    };
  }
}

#endif