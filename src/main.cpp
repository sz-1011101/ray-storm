#include "utility/common.hpp"
#include "scene/Scene.h"

#include "camera/PinholeCamera.h"
#include "camera/ThinLensCamera.h"
#include "camera/LensFactory.h"

#include "scene/TestSceneFactory.h"
#include "renderer/PathTraceSampler.h"
#include "renderer/DefaultRenderer.h"
#include "utility/Window.h"
#include "utility/RenderedData.h"

int main(int argc, char* argv[])
{

  using namespace ray_storm;

  utility::RenderedDataPtr rd(new utility::RenderedData());
  utility::Window window;
  window.setRenderedData(rd);
  rd->setWindow(&window);

  camera::AbstractSingleImageCameraPtr camera(new camera::PinholeCamera(
    camera::CameraSetupPtr(
      new camera::CameraSetup(
        glm::vec3(0, 5.0f, 9.5f),
        glm::vec3(0, 5, -10),
        glm::vec3(0, 1, 0),
        1.0f,
        75.0f
        )
      ),
      rd,
      200,
      200
    )
  );

  renderer::AbstractRadianceSamplerPtr pts(
    new renderer::PathTraceSampler(
      renderer::PathTraceSampler::METHOD::BIDIRECTIONAL));

  scene::ScenePtr scene = scene::TestSceneFactory::createCornellBox(false, true);
  
  renderer::DefaultRenderer dr(scene, camera, pts, 25);

  dr.render();

  window.wait();
  
  return 0;
}