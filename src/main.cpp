#include "utility/common.hpp"
#include "scene/Scene.h"

#include "camera/PinholeCamera.h"
#include "scene/TestSceneFactory.h"
#include "renderer/PathTracer.h"
#include "utility/Window.h"
#include "utility/RenderedData.h"

int main(int argc, char* argv[])
{

  using namespace ray_storm;

  // camera
  camera::AbstractCameraPtr camera(new camera::PinholeCamera(
    glm::vec3(0, 5, 9.5f), glm::vec3(0, 5, -5), glm::vec3(0, 1, 0), 1.0f, 65.0f));

  scene::ScenePtr scene = scene::TestSceneFactory::createCornellBox();

  utility::RenderedDataPtr rd(new utility::RenderedData(512, 512));
  utility::Window window;
  window.setRenderedData(rd);
  rd->setWindow(&window);

  renderer::PathTracer pt(scene, camera);
  pt.setRenderedData(rd);

  pt.render();

  window.wait();
  
  return 0;
}