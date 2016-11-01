#ifndef ABSTRACT_SINGLE_IMAGE_CAMERA_H_
#define ABSTRACT_SINGLE_IMAGE_CAMERA_H_

#include "camera/AbstractCamera.h"
#include "utility/RenderedData.h"
#include "utility/SamplePlate.hpp"

namespace ray_storm
{
  namespace camera
  {
    class AbstractSingleImageCamera : public AbstractCamera
    {
    public:

      AbstractSingleImageCamera(const utility::RenderedDataPtr &renderedData, uint32_t width, uint32_t height);

      virtual ~AbstractSingleImageCamera() {};

      void gatherSample(const glm::vec2 &xy, const glm::vec3 &sample);

      void signal();

      uint32_t getWidth();

      uint32_t getHeight();

    protected:

      utility::RenderedDataPtr renderedData;

      utility::SamplePlate samples;
      
    };

    typedef std::shared_ptr<AbstractSingleImageCamera> AbstractSingleImageCameraPtr;
  }
}

#endif