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

      Rectangle(const glm::vec3 &origin, const glm::vec3 normal,
        const glm::vec3 &wAxis, float width, float height, materials::AbstractMaterialPtr &material) :
        Object(material), plane(origin, normal, material)
      {
        this->normal = glm::normalize(normal);
        this->wAxis = glm::normalize(wAxis);
        this->hAxis = glm::normalize(glm::cross(this->normal, wAxis));

        this->width = width;
        this->height = height;
      }

      inline bool intersect(const Ray &ray, Intersection<Object> &intersection)
      {
        Intersection<Object> pInters;

        if (!this->plane.intersect(ray, pInters))
        {
          return false;
        }

        const glm::vec3 &pIntersPnt = pInters.intersection.position;

        const glm::vec3 rInters = pIntersPnt - this->origin;
        const float wAxisDot = glm::dot(rInters, this->wAxis);
        const float hAxisDot = glm::dot(rInters, this->hAxis);

        // inside the rectangle?
        if (wAxisDot < 0.0f || wAxisDot >= this->width || hAxisDot < 0.0f || hAxisDot >= this->height)
        {
          return false;
        }
        
        intersection = pInters;
        intersection.intersected = this;

        return true;
      }

    private:

      glm::vec3 origin;
      glm::vec3 normal;
      glm::vec3 wAxis;
      glm::vec3 hAxis;

      float width;
      float height;

      // the rectangle's plane
      Plane plane;
      
    };

  }

}

#endif