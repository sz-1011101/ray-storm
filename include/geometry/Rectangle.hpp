#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "geometry/Object.h"
#include "geometry/Plane.hpp"

namespace ray_storm
{
  namespace geometry
  {

    class Rectangle : public Object
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

      Rectangle(const RectParams &rectParams, materials::MaterialPtr &material) :
        Object(material), rectParams(rectParams), plane(rectParams.origin, rectParams.calcNormal())
      {
        this->normal = rectParams.calcNormal();
      }

      inline bool intersect(const Ray &ray, Intersection<Object> &intersection)
      {
        Intersection<Plane> pInters;

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
        const RectParams &rp = this->rectParams;
        return  rp.origin + u*rp.wAxis*rp.width + v*rp.hAxis*rp.height;
      }

    private:

      RectParams rectParams;
      glm::vec3 normal;

      // the rectangle's plane
      Plane plane;
      
    };

  }

}

#endif