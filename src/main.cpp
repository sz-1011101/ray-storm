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
const std::string ITERATIONS_ARG = "iterations";
const std::string WIDTH_ARG = "width";
const std::string HEIGHT_ARG = "height";
const std::string METHOD_ARG = "method";
const std::string METHOD_PT = "PT";
const std::string METHOD_DPT = "DPT";
const std::string METHOD_D_MIS_PT = "D_MIS_PT";
const std::string METHOD_LPT = "LPT";
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
  else if (name == METHOD_LPT)
  {
    return integrators::IntegratorGeneratorFactory::createLightPathTracerGenerator();
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
  (SPP_ARG.c_str(), boost::program_options::value<int>(), "Samples per pixel")
  (ITERATIONS_ARG.c_str(), boost::program_options::value<int>(), "Progressive iterations to do.")
  (WIDTH_ARG.c_str(), boost::program_options::value<int>(), "Image width in pixels.")
  (HEIGHT_ARG.c_str(), boost::program_options::value<int>(), "Image height in pixels.")
  (METHOD_ARG.c_str(), boost::program_options::value<std::string>(), "Integration method."); // TODO show methods

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help"))
  {
    std::cout << "ray-storm - Yet Another CPU Path Tracer\n"<< desc << '\n';
    return 0;
  }

  int spp = 10;
  int progressiveIterations = 1;
  int width = 500;
  int height = 500;
  integrators::AbstractIntegratorGeneratorPtr pts;

  if (vm.count(SPP_ARG.c_str()))
  {
    spp = vm[SPP_ARG.c_str()].as<int>();
    std::cout << "samples per pixel set to " << spp << '\n';
  }

  if (vm.count(ITERATIONS_ARG.c_str()))
  {
    progressiveIterations = vm[ITERATIONS_ARG.c_str()].as<int>();
    std::cout << "progressive iterations set to " << progressiveIterations << '\n';
  }

  if (vm.count(WIDTH_ARG.c_str()) && vm.count(HEIGHT_ARG.c_str()))
  {
    width = vm[WIDTH_ARG.c_str()].as<int>();
    height = vm[HEIGHT_ARG.c_str()].as<int>();
    std::cout << "image resolution set to " << width << ", " << height << '\n';
  }
  else if (vm.count(WIDTH_ARG.c_str()) || vm.count(HEIGHT_ARG.c_str()))
  {
    std::cout << "give both " << WIDTH_ARG << " and " << HEIGHT_ARG << '\n';
    return -1;
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

  if (spp <= 0 || width <= 0 || height <= 0 || progressiveIterations <= 0) {
    std::cout << "invalid argument(s)\n";
    return -1;
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
        static_cast<float>(width)/height,
        75.0f
        )
      ),
      rd,
      width,
      height
    )
  );

  scene::ScenePtr scene = scene::TestSceneFactory::createCornellBox(false, true);
  
  renderer::DefaultRenderer dr(scene, camera, pts, spp, progressiveIterations);

  dr.render();

  window.wait();
  
  return 0;
}