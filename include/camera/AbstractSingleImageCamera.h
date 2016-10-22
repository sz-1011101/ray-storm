#ifndef ABSTRACT_SINGLE_IMAGE_CAMERA_H_
#define ABSTRACT_SINGLE_IMAGE_CAMERA_H_

#include "camera/AbstractCamera.h"
#include "utility/RenderedData.h"

namespace ray_storm
{
  namespace camera
  {
    class AbstractSingleImageCamera : public AbstractCamera
    {
    public:

      AbstractSingleImageCamera(const utility::RenderedDataPtr &renderedData);

      virtual ~AbstractSingleImageCamera() {};

      void setTile(int xOrg, int yOrg, const utility::Image &tile);

      uint32_t getWidth();

      uint32_t getHeight();

    protected:

      utility::RenderedDataPtr renderedData;
      
    };

    typedef std::shared_ptr<AbstractSingleImageCamera> AbstractSingleImageCameraPtr;
  }
}

#endif