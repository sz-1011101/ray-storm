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

      Sphere(
        const glm::vec3 &position, 
        float radius, materials::MaterialPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      ) : Object(material, emittance)
      {
        this->position = position;
        this->radius = radius;
      }

      inline bool intersect(const Ray &ray, Intersection<Object> &intersection) {
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
        glm::vec3 normal = glm::normalize(iPoint - this->position);

        // flipp normal towards ray origin
        if (glm::dot(normal, ray.direction) > 0.0f)
        {
          normal = -normal;
        }
        intersection.intersection = SimpleIntersection (iPoint, normal);
        return true;
      }

      float getSurfaceArea()
      {
        return 4.0f*static_cast<float>(M_PI)*this->radius*this->radius;
      }

      glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper)
      {
        return this->position + randHelper.drawUniformRandomSphereDirection()*this->radius;
      }

      float getPDF()
      {
        return 1.0f/this->getSurfaceArea();
      }

    protected:

      glm::vec3 position;

      float radius;

    };
  }
}

#endif