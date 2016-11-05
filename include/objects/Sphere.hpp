#ifndef SPHERE_H_
#define SPHERE_H_

#include "objects/Emitter.h"
#include "geometry/SpherePrimitive.hpp"

namespace ray_storm
{
  namespace objects
  {
    
    class Sphere : public Emitter
    {

    public:

      Sphere(
        const glm::vec3 &position, 
        float radius, const materials::MaterialPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      ) : Emitter(material, emittance), sphere(position, radius)
      {
        this->bbox = geometry::AxisAlignedBox(position - glm::vec3(radius), glm::vec3(2.0f*radius));
      }

      bool intersect(const geometry::Ray &ray, geometry::Intersection<Object> &intersection) {
        geometry::Intersection<geometry::SpherePrimitive> _intersection;
        if (this->sphere.intersect(ray, _intersection))
        {
          intersection.intersection = _intersection.intersection;
          intersection.intersected = this;
          intersection.t = _intersection.t;
          return true;
        }
        return false;
      }

      float getSurfaceArea()
      {
        return this->sphere.getSurfaceArea();
      }

      glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper)
      {
        return this->sphere.getPosition() + randHelper.drawUniformRandomSphereDirection()*(this->sphere.getRadius() + Emitter::SURFACE_POINT_OFFSET);
      }

      void drawRandomRay(random::RandomizationHelper &randHelper, RaySample &raySample)
      {
        raySample.randRay.ray.origin = this->drawRandomSurfacePoint(randHelper);
        const glm::vec3 n = glm::normalize(raySample.randRay.ray.origin - this->sphere.getPosition());
        raySample.randRay.ray.direction = randHelper.drawUniformRandomHemisphereDirection(n);
        raySample.randRay.PDF = this->getPDF()*random::RandomizationHelper::uniformRandomHemispherePDF();
        raySample.emittance = this->getEmittance(raySample.randRay.ray.direction, n);
      }

      float getPDF()
      {
        return 1.0f/this->getSurfaceArea();
      }

    protected:

      geometry::SpherePrimitive sphere;

    };
  }
}

#endif