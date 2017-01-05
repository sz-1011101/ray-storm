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
        this->bbox = this->sphere.computeBBox();
      }

      Sphere(
        const glm::vec3 &position, 
        float radius, const materials::MaterialPtr &material,
        const textures::Abstract2DTexturePtr<glm::vec3> &emittance
      ) : Emitter(material, emittance), sphere(position, radius)
      {
        this->bbox = this->sphere.computeBBox();
      }

      bool intersect(const geometry::Ray &ray, geometry::Intersection<Object> &intersection) {
        geometry::Intersection<geometry::SpherePrimitive> _intersection;
        if (this->sphere.intersect(ray, _intersection))
        {
          intersection.intersection = _intersection.intersection;
          intersection.intersection.normal = this->modifyNormal(
            intersection.intersection.normal, intersection.intersection.texCoords
           );
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

      PointSample drawRandomSurfacePoint(random::RandomizationHelper &randHelper)
      {
        const glm::vec3 dir = randHelper.drawUniformRandomSphereDirection();
        const glm::vec2 uv = utility::Math::cartesianToSphericalToUV(dir);
        return PointSample(
          this->sphere.getPosition() + dir*(this->sphere.getRadius() + Emitter::SURFACE_POINT_OFFSET),
          uv
        );
      }

      void drawRandomRay(random::RandomizationHelper &randHelper, RaySample &raySample)
      {
        PointSample ps = this->drawRandomSurfacePoint(randHelper);
        raySample.randRay.ray.origin = ps.point;
        const glm::vec3 n = glm::normalize(raySample.randRay.ray.origin - this->sphere.getPosition());
        raySample.randRay.ray.direction = randHelper.drawCosineDistributedDirection(n);
        raySample.randRay.PDF = this->getPDF(raySample.randRay.ray.direction, n);
        raySample.emittance = this->getEmittance(raySample.randRay.ray.direction, n, ps.uv);
      }

      float getPDF(const glm::vec3 &l, const glm::vec3 &n)
      {
        return glm::dot(l, n) > 0.0f ? 1.0f/this->getSurfaceArea() : 0.0f;
      }

    protected:

      geometry::SpherePrimitive sphere;

    };
  }
}

#endif