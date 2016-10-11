#ifndef DEFAULT_RENDERER_H_
#define DEFAULT_RENDERER_H_

#include <memory>

#include "renderer/AbstractRenderer.h"
#include "renderer/AbstractRadianceSampler.h"

namespace ray_storm
{
  namespace renderer
  {
    class DefaultRenderer : public AbstractRenderer
    {
    public:
      DefaultRenderer(
        const scene::ScenePtr &scene,
        const camera::AbstractCameraPtr &camera,
        const AbstractRadianceSamplerPtr &sampler,
        uint32_t samples
      );

      virtual void render();

    private:

      cv::Mat renderedImage;

      scene::ScenePtr scene;

      camera::AbstractCameraPtr camera;

      AbstractRadianceSamplerPtr sampler;

      uint32_t samples;
      
    };
  }
}

#endif