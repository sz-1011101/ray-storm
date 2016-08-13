#ifndef PATH_TRACER_H_
#define PATH_TRACER_H_

#include <opencv2/core/core.hpp>

#include "utility/common.hpp"
#include "renderer/AbstractRenderer.h"
#include "utility/RenderedData.h"
#include "random/RandomizationHelper.h"

namespace ray_storm
{
  namespace renderer
  {

    class PathTracer : public AbstractRenderer
    {

    public:

      PathTracer();

      PathTracer(scene::ScenePtr &scene, camera::AbstractCameraPtr &camera);

      void setScene(scene::ScenePtr &scene);

      void setCamera(camera::AbstractCameraPtr &camera);

      void render();

    private:

      cv::Mat renderedImage;

      scene::ScenePtr scene;

      camera::AbstractCameraPtr camera;

      glm::vec3 sky;

      glm::vec3 walkPath(const geometry::Ray &initialRay, 
        random::RandomizationHelper::MTEngine &engine);
      
    };
  }
}

#endif