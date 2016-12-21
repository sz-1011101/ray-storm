#include <string>
#include <sstream>

#include "boost/program_options.hpp" 

#include "utility/common.hpp"
#include "scene/Scene.h"

#include "camera/PinholeCamera.h"
#include "camera/ThinLensCamera.h"
#include "camera/LensFactory.h"

#include "scene/TestSceneFactory.h"
#include "integrators/IntegratorGeneratorFactory.h"
#include "renderer/DefaultRenderer.h"
#include "utility/Window.h"
#include "utility/RenderedData.h"

using namespace ray_storm;

const std::string SPP_ARG = "spp";
const std::string METHOD_ARG = "method";
const std::string METHOD_PT = "PT";
const std::string METHOD_DPT = "DPT";
const std::string METHOD_D_MIS_PT = "D_MIS_PT";
const std::string METHOD_BIDIR_PT = "BIDIR_PT";

integrators::AbstractIntegratorGeneratorPtr parseMethod(const std::string &name)
{
  if (name == METHOD_PT)
  {
    return integrators::IntegratorGeneratorFactory::createPathTracerGenerator();
  }
  else if (name == METHOD_DPT)
  {
    return integrators::IntegratorGeneratorFactory::createDirectLightingPathTracerGenerator(false);
  }
  else if (name == METHOD_D_MIS_PT)
  {
    return integrators::IntegratorGeneratorFactory::createDirectLightingPathTracerGenerator(true);
  }
  else if (name == METHOD_BIDIR_PT)
  {
    return integrators::IntegratorGeneratorFactory::createBidirectionalPathTracerGenerator();
  }

  return integrators::AbstractIntegratorGeneratorPtr();
}

int main(int argc, char* argv[])
{

  boost::program_options::options_description desc("Options");
  desc.add_options()
  ("help", "show help message")
  (SPP_ARG.c_str(), boost::program_options::value<int>(), "samples per pixel")
  (METHOD_ARG.c_str(), boost::program_options::value<std::string>(), "integration method."); // TODO show methods

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help"))
  {
    std::cout << "ray-storm - Yet Another CPU Path Tracer\n"<< desc << '\n';
    return 0;
  }

  int spp = 10;
  integrators::AbstractIntegratorGeneratorPtr pts;

  if (vm.count(SPP_ARG.c_str()))
  {
    spp = vm[SPP_ARG.c_str()].as<int>();
    std::cout << "samples per pixel set to " << spp << '\n';
  }

  if (vm.count(METHOD_ARG.c_str()))
  {
    const std::string method = vm[METHOD_ARG.c_str()].as<std::string>();
    std::cout << "integration method set to " << method << '\n';
    pts = parseMethod(method);
    if (pts == nullptr)
    {
      std::cout << "method not recognized\n";
      return -1;
    }
  }
  else
  {
    pts = integrators::IntegratorGeneratorFactory::createPathTracerGenerator();
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

  scene::ScenePtr scene = scene::TestSceneFactory::createCornellBox(false, true);
  
  renderer::DefaultRenderer dr(scene, camera, pts, spp);

  dr.render();

  window.wait();
  
  return 0;
}