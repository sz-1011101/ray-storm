#include "scene/Scene.h"
#include "geometry/Sphere.hpp"
#include "geometry/Plane.hpp"
#include "camera/PinholeCamera.h"
#include "materials/Lambertian.hpp"
#include "renderer/PathTracer.h"
#include "utility/Window.h"
#include "utility/RenderedData.h"

int main(int argc, char* argv[])
{

  using namespace ray_storm;

  scene::ScenePtr scene(new scene::Scene());

  // camera
  camera::AbstractCameraPtr camera(new camera::PinholeCamera(glm::vec3(-7,5,5), glm::vec3(0,0,0), glm::vec3(0, 1, 0), 1.0f, 75.0f));
  
  // materials
  materials::AbstractMaterialPtr mat1(new materials::Lambertian(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr mat2(new materials::Lambertian(glm::vec3(0.4f), glm::vec3(10.0f)));
  materials::AbstractMaterialPtr mat3(new materials::Lambertian(glm::vec3(1.0f), glm::vec3(0.0f)));

  // scene objects
  geometry::ObjectPtr sphere1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(1.0f, 0.0, 0.0f), 1.0f, mat1));
  geometry::ObjectPtr sphere2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-0.5f, 0.0f, 0.0f), 0.25f, mat2));
  geometry::ObjectPtr plane = geometry::ObjectPtr(new geometry::Plane(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0, 1, 0), mat3));

  scene->add(sphere1);
  scene->add(sphere2);
  scene->add(plane);
  scene->finalize();

  utility::RenderedDataPtr rd(new utility::RenderedData());
  utility::Window window;
  window.setRenderedData(rd);
  rd->setWindow(&window);

  rd->initialize(512, 512);

  renderer::PathTracer pt(scene, camera);
  pt.setRenderedData(rd);

  pt.render();

  window.wait();
  
  return 0;
}