#ifndef SPHERE_H_
#define SPHERE_H_

#include "objects/Emitter.h"
#include "utility/common.hpp"
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
        float rad = this->sphere.getRadius();
        return 4.0f*static_cast<float>(M_PI)*rad*rad;
      }

      glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper)
      {
        return this->sphere.getPosition() + randHelper.drawUniformRandomSphereDirection()*(this->sphere.getRadius() + Emitter::SURFACE_POINT_OFFSET);
      }

      void drawRandomRay(random::RandomizationHelper &randHelper, random::RandomRay &randRay)
      {
        randRay.ray.origin = this->drawRandomSurfacePoint(randHelper);
        randRay.ray.direction = randHelper.drawUniformRandomHemisphereDirection(glm::normalize(randRay.ray.origin - this->sphere.getPosition()));
        randRay.PDF = this->getPDF()*random::RandomizationHelper::uniformRandomHemispherePDF();
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