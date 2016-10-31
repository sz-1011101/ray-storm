#ifndef SPHERE_H_
#define SPHERE_H_

#include "geometry/Emitter.h"
#include "utility/common.hpp"

namespace ray_storm
{
  namespace geometry
  {
    
    class Sphere : public Emitter
    {

    public:

      Sphere(
        const glm::vec3 &position, 
        float radius, const materials::MaterialPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      ) : Emitter(material, emittance)
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
        
        const glm::vec3 spherical = utility::Math::cartesianToSpherical(iPoint - position);
        intersection.intersection = SimpleIntersection (iPoint, glm::normalize(iPoint - this->position), glm::vec2(spherical.y/(2.0f*M_PI), spherical.z/M_PI));
        return true;
      }

      float getSurfaceArea()
      {
        return 4.0f*static_cast<float>(M_PI)*this->radius*this->radius;
      }

      glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper)
      {
        return this->position + randHelper.drawUniformRandomSphereDirection()*(this->radius + Emitter::SURFACE_POINT_OFFSET);
      }

      void drawRandomRay(random::RandomizationHelper &randHelper, random::RandomRay &randRay)
      {
        randRay.ray.origin = this->drawRandomSurfacePoint(randHelper);
        randRay.ray.direction = randHelper.drawUniformRandomHemisphereDirection(glm::normalize(this->position - randRay.ray.origin));
        randRay.PDF = this->getPDF()*random::RandomizationHelper::uniformRandomHemispherePDF();
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