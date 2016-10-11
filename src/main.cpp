#include "utility/common.hpp"
#include "scene/Scene.h"

#include "camera/PinholeCamera.h"
#include "camera/ThinLensCamera.h"

#include "scene/TestSceneFactory.h"
#include "renderer/PathTraceSampler.h"
#include "renderer/DefaultRenderer.h"
#include "utility/Window.h"
#include "utility/RenderedData.h"

int main(int argc, char* argv[])
{

  using namespace ray_storm;

  // camera
  camera::AbstractCameraPtr camera(new camera::ThinLensCamera(camera::ThinLensCameraSetupPtr(
    new camera::ThinLensCameraSetup(
      glm::vec3(0, 5.0f, 9.5f),
      glm::vec3(0, 5, -10),
      glm::vec3(0, 1, 0),
      1.0f,
      75.0f,
      0.095f,
      1.0f,
      geometry::Plane(glm::vec3(0, 0, 1.0f), glm::normalize(glm::vec3(0, 1, 1)))
    )
  )));

  scene::ScenePtr scene = scene::TestSceneFactory::createCornellBox();
  //scene::ScenePtr scene = scene::TestSceneFactory::createReflectionTest();
  
  utility::RenderedDataPtr rd(new utility::RenderedData(200, 200));
  utility::Window window;
  window.setRenderedData(rd);
  rd->setWindow(&window);

  renderer::AbstractRadianceSamplerPtr pts(new renderer::PathTraceSampler(renderer::PathTraceSampler::METHOD::DIRECT_BOUNCE));
  renderer::DefaultRenderer dr(scene, camera, pts, 100);
  dr.setRenderedData(rd);

  dr.render();

  window.wait();
  
  return 0;
}