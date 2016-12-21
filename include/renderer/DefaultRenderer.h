#ifndef DEFAULT_RENDERER_H_
#define DEFAULT_RENDERER_H_

#include <memory>

#include "renderer/AbstractRenderer.h"
#include "integrators/AbstractIntegrator.h"
#include "camera/AbstractSingleImageCamera.h"

namespace ray_storm
{
  namespace renderer
  {
    class DefaultRenderer : public AbstractRenderer
    {
    public:
      DefaultRenderer(
        const scene::ScenePtr &scene,
        const camera::AbstractSingleImageCameraPtr &camera,
        const integrators::AbstractIntegratorPtr &integrator,
        uint32_t samples
      );

      virtual void render();

    private:

      cv::Mat renderedImage;

      scene::ScenePtr scene;

      camera::AbstractSingleImageCameraPtr camera;

      integrators::AbstractIntegratorPtr integrator;

      uint32_t samples;
      
    };
  }
}

#endif