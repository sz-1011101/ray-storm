#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "objects/Object.h"
#include "geometry/Plane.hpp"

namespace ray_storm
{
  namespace objects
  {

    class Rectangle : public Emitter
    {
    public:

      struct RectParams
      {
        glm::vec3 origin;
        glm::vec3 wAxis;
        glm::vec3 hAxis;
        float width;
        float height;

        RectParams(const glm::vec3 &origin,
                   const glm::vec3 &wAxis,
                   const glm::vec3 &hAxis,
                   float width,
                   float height)
        {
          this->origin = origin;
          this->wAxis = glm::normalize(wAxis);
          this->hAxis = glm::normalize(hAxis);
          this->width = width;
          this->height = height;
        }

        glm::vec3 calcNormal() const
        {
          return cross(this->hAxis, this->wAxis);
        }
      };

      Rectangle(
        const RectParams &rectParams,
        materials::MaterialPtr &material,
        const glm::vec3 &emittance = glm::vec3(0.0f)
      ) : Emitter(material, emittance), rectParams(rectParams), plane(rectParams.origin, rectParams.calcNormal())
      {
        this->normal = rectParams.calcNormal();
        this->wSide = rectParams.wAxis*rectParams.width;
        this->hSide = rectParams.hAxis*rectParams.height;
        this->bbox.cover(this->rectParams.origin);
        this->bbox.cover(this->rectParams.origin + this->wSide);
        this->bbox.cover(this->rectParams.origin + this->hSide);
        this->bbox.cover(this->rectParams.origin + this->wSide + this->hSide);
      }

      inline bool intersect(const geometry::Ray &ray, geometry::Intersection<Object> &intersection)
      {
        geometry::Intersection<geometry::Plane> pInters;

        // only intersect on one side, other side is invisible
        if (!this->plane.intersect(ray, pInters))
        {
          return false;
        }

        const glm::vec3 &pIntersPnt = pInters.intersection.position;

        const glm::vec3 rInters = pIntersPnt - this->rectParams.origin;
        const float wAxisDot = glm::dot(rInters, this->rectParams.wAxis);
        const float hAxisDot = glm::dot(rInters, this->rectParams.hAxis);

        // inside the rectangle?
        if (wAxisDot < 0.0f || wAxisDot >= this->rectParams.width || hAxisDot < 0.0f || hAxisDot >= this->rectParams.height)
        {
          return false;
        }
        
        intersection.intersection = pInters.intersection;
        intersection.intersection.texCoords = glm::vec2(wAxisDot/this->rectParams.width, hAxisDot/this->rectParams.height);
        intersection.t = pInters.t;
        intersection.intersected = this;

        return true;
      }

      float getSurfaceArea()
      {
        return this->rectParams.width*this->rectParams.height;
      }

      glm::vec3 drawRandomSurfacePoint(random::RandomizationHelper &randHelper)
      {
        float u = randHelper.drawUniformRandom();
        float v = randHelper.drawUniformRandom();
        return this->rectParams.origin + u*this->wSide + v*this->hSide + this->normal*Emitter::SURFACE_POINT_OFFSET;
      }

      void drawRandomRay(random::RandomizationHelper &randHelper, RaySample &raySample)
      {
        raySample.randRay.ray.origin = this->drawRandomSurfacePoint(randHelper);
        raySample.randRay.ray.direction = randHelper.drawUniformRandomHemisphereDirection(glm::normalize(this->normal));
        raySample.randRay.PDF = this->getPDF()*random::RandomizationHelper::uniformRandomHemispherePDF();
        raySample.emittance = this->getEmittance(raySample.randRay.ray.direction, this->normal);
      }

      float getPDF()
      {
        return 1.0f/this->getSurfaceArea();
      }

    private:

      RectParams rectParams;
      glm::vec3 normal;

      // side vectors
      glm::vec3 wSide;
      glm::vec3 hSide;

      // the rectangle's plane
      geometry::Plane plane;
      
    };

  }

}

#endif