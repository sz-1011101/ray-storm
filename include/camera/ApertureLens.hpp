#ifndef APERTURE_LENS_H_
#define APERTURE_LENS_H_

#include <memory>
#include <opencv2/imgproc/imgproc.hpp>

#include "camera/SimpleLens.hpp"

namespace ray_storm
{
  namespace camera
  {
    class ApertureLens : public SimpleLens
    {
    public:

      ApertureLens(const std::vector<glm::vec2> &shape) : SimpleLens(this->findDiameter(shape))
      {
        for (const glm::vec2 &p : shape)
        {
          this->shape.push_back(cv::Point2f(p.x, p.y));
        }
      }

      virtual void spawnPoints(
        std::size_t amount,
        std::vector<glm::vec2> &lensPoints,
        random::RandomizationHelper &randHelper
      )
      {
        lensPoints.clear();
        lensPoints.reserve(amount);
        while (amount > lensPoints.size())
        {
          glm::vec2 randPnt(randHelper.drawUniformRandom(), randHelper.drawUniformRandom());
          randPnt = (2.0f*randPnt - 1.0f)*this->radius;
          float len = glm::length(randPnt);
          if (len < this->radius && cv::pointPolygonTest(this->shape, cv::Point2f(randPnt.x, randPnt.y), false) >= 0.0f)
          {
            lensPoints.push_back(randPnt);
          }
        }
      }

    protected:
      // aperture contour shape
      std::vector<cv::Point2f> shape;

    private:

      float findDiameter(const std::vector<glm::vec2> &shape)
      {
        float maxRadius = 0.0f;
        for (const glm::vec2 &p : shape)
        {
          maxRadius = std::max(maxRadius, glm::length(p));
        }
        return maxRadius*2.0f;
      }
      
    };

    typedef std::shared_ptr<ApertureLens> ApertureLensPtr;
  }
}

#endif