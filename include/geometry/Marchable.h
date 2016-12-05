#ifndef MARCHABLE_H_
#define MARCHABLE_H_

#include <memory>

#include "utility/common.hpp"

namespace ray_storm
{
  namespace geometry
  {
    class Marchable
    {
    public:

      virtual ~Marchable() {};
      
      // object space distance
      virtual float distance(const glm::vec3 &p) const = 0;

      virtual const glm::vec3 &getCenter() const = 0;

      virtual glm::vec3 estimateNormal(const glm::vec3 &p) {
        // see https://www.reddit.com/r/twotriangles/comments/1hy5qy/tutorial_1_writing_a_simple_distance_field/
        const float eps = 0.1f;
        const glm::vec3 xDir = glm::vec3(eps, 0, 0);
        const glm::vec3 yDir = glm::vec3(0, eps, 0);
        const glm::vec3 zDir = glm::vec3(0, 0, eps);
        const float x = this->distance(p + xDir) - this->distance(p - xDir);
        const float y = this->distance(p + yDir) - this->distance(p - yDir);
        const float z = this->distance(p + zDir) - this->distance(p - zDir);
        return glm::normalize(glm::vec3(x, y, z));
      }
    };

    typedef std::shared_ptr<Marchable> MarchablePtr;
  }
}

#endif