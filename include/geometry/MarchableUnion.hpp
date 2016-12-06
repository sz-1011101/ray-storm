#ifndef MARCHABLE_UNION_H_
#define MARCHABLE_UNION_H_

#include <vector>
#include <memory>

#include "utility/common.hpp"
#include "geometry/Marchable.h"

namespace ray_storm
{
  namespace geometry
  {
    class MarchableUnion : public Marchable
    {
    public:

      MarchableUnion(const glm::vec3 &center) : center(center) {};

      // Marchables are relative to the center of this Marchable
      void add(const MarchablePtr &marchable)
      {
        this->marchableUnion.push_back(marchable);
      }

      float distance(const glm::vec3 &p) const
      {
        float min = 999999.0f;
        bool first = true;
        for (const MarchablePtr &m : this->marchableUnion)
        {
          // object space of the Marchable
          const glm::vec3 p2 = p - m->getCenter();

          min = first ? m->distance(p2) : std::min(min, m->distance(p2));
          first = false;
        }

        return min;

      }

      const glm::vec3 &getCenter() const
      {
        return this->center;
      }

      AxisAlignedBox computeMarchingCube() const
      {
        AxisAlignedBox bbox;
        for (const MarchablePtr &m : this->marchableUnion)
        {
          AxisAlignedBox mbbox = m->computeMarchingCube();
          bbox.cover(mbbox);
        }
        bbox.translate(this->center);
        return bbox;
      }

    private:

      glm::vec3 center;

      std::vector<MarchablePtr> marchableUnion;
      
    };

    typedef std::shared_ptr<MarchableUnion> MarchableUnionPtr;
  }
}

#endif