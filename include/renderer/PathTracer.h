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

      enum METHOD
      {
        NAIVE = 0,
        DIRECT,
        DIRECT_BOUNCE
      };

      struct Settings
      {
        uint32_t samples;
        METHOD method;

        Settings(uint32_t samples = 64, METHOD method = DIRECT_BOUNCE)
        {
          this->samples = samples;
          this->method = method;
        }
      };

      PathTracer(scene::ScenePtr &scene, camera::AbstractCameraPtr &camera, const Settings &settings);

      void setScene(scene::ScenePtr &scene);

      void setCamera(camera::AbstractCameraPtr &camera);

      void render();

    private:

      Settings settings;

      cv::Mat renderedImage;

      scene::ScenePtr scene;

      camera::AbstractCameraPtr camera;

      glm::vec3 sky;

      glm::vec3 walkPath(const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper);

      glm::vec3 walkPathDirectLighting(const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper);

      glm::vec3 walkPathDirectLighting2(const geometry::Ray &initialRay, 
        random::RandomizationHelper &randHelper);
      
    };
  }
}

#endif