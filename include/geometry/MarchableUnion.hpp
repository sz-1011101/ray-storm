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

      MarchableUnion(const glm::vec3 &center, float smoothingK)
        : center(center), smoothingK(smoothingK)
      {

      };

      // Marchables are relative to the center of this Marchable
      void add(const MarchablePtr &marchable)
      {
        this->marchableUnion.push_back(marchable);
      }

      float distance(const glm::vec3 &p) const
      {
        // See http://iquilezles.org/www/articles/smin/smin.htm for the smoothing
        float res = 0.0f;
        for (const MarchablePtr &m : this->marchableUnion)
        {
          const glm::vec3 mp = p - this->center;
          res += std::exp(-smoothingK*m->distance(mp));
        }

        return -std::log(res)/smoothingK;
      }

      glm::vec2 computeTexCoords(const glm::vec3 &p)
      {
        return utility::Math::cartesianToSphericalToUV(p - this->center);
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

      float smoothingK;

      std::vector<MarchablePtr> marchableUnion;
      
    };

    typedef std::shared_ptr<MarchableUnion> MarchableUnionPtr;
  }
}

#endif