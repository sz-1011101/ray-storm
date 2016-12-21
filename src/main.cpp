#include <string>

#include "boost/program_options.hpp" 

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

const std::string SPP_ARG = "spp";

int main(int argc, char* argv[])
{

  using namespace ray_storm;

  boost::program_options::options_description desc("Options");
  desc.add_options()
  ("help", "show help message")
  (SPP_ARG.c_str(), boost::program_options::value<int>(), "samples per pixel");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help"))
  {
    std::cout << "ray-storm - Yet Another CPU Path Tracer\n"<< desc << '\n';
    return 0;
  }

  int spp = 10;
  if (vm.count(SPP_ARG.c_str()))
  {
    spp = vm[SPP_ARG.c_str()].as<int>();
    std::cout << "samples per pixel set to " << spp << '\n';
  }

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
      500,
      500
    )
  );

  renderer::AbstractRadianceSamplerPtr pts(
    new renderer::PathTraceSampler(
      renderer::PathTraceSampler::METHOD::BIDIRECTIONAL));

  scene::ScenePtr scene = scene::TestSceneFactory::createCornellBox(false, true);
  
  renderer::DefaultRenderer dr(scene, camera, pts, spp);

  dr.render();

  window.wait();
  
  return 0;
}