#ifndef DEFAULT_RENDERER_H_
#define DEFAULT_RENDERER_H_

#include <memory>

#include "renderer/AbstractRenderer.h"
#include "integrators/AbstractIntegratorGenerator.h"
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
        const integrators::AbstractIntegratorGeneratorPtr &integratorGen,
        uint32_t samples,
        uint32_t progressiveIterations
      );

      virtual void render();

    private:

      cv::Mat renderedImage;

      scene::ScenePtr scene;

      camera::AbstractSingleImageCameraPtr camera;

      integrators::AbstractIntegratorGeneratorPtr integratorGen;

      uint32_t samples;

      uint32_t progressiveIterations;

      void fill(int n, std::vector<integrators::AbstractIntegratorPtr> &integrators);
      
    };
  }
}

#endif