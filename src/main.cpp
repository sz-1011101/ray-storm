#include "utility/common.hpp"
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
  camera::AbstractCameraPtr camera(new camera::PinholeCamera(
    glm::vec3(2,2,2), glm::vec3(0,0,0), glm::vec3(0, 1, 0), 1.0f, 75.0f));

  // materials
  materials::AbstractMaterialPtr eMat(new materials::Lambertian(glm::vec3(0.0f), glm::vec3(30.0f)));
  materials::AbstractMaterialPtr matWhite(new materials::Lambertian(glm::vec3(1), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matR(new materials::Lambertian(glm::vec3(1, 0, 0), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matG(new materials::Lambertian(glm::vec3(0, 1, 0), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matB(new materials::Lambertian(glm::vec3(0, 0, 1), glm::vec3(0.0f)));

  // scene objects
  geometry::ObjectPtr sphereX = geometry::ObjectPtr(
    new geometry::Sphere(glm::vec3(1.0f, 0.0, 0.0f), 0.5f, matR));
  geometry::ObjectPtr sphereY = geometry::ObjectPtr(
    new geometry::Sphere(glm::vec3(0.0f, 1.0, 0.0f), 0.5f, matG));
  geometry::ObjectPtr sphereZ = geometry::ObjectPtr(
    new geometry::Sphere(glm::vec3(0.0f, 0.0, 1.0f), 0.5f, matB));
  geometry::ObjectPtr sphereLight = geometry::ObjectPtr(
    new geometry::Sphere(glm::vec3(3.0f, 0.0f, 0.0f), 0.5f, eMat));
  geometry::ObjectPtr plane1 = geometry::ObjectPtr(
    new geometry::Plane(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0, 1, 0), matWhite));
  geometry::ObjectPtr plane2 = geometry::ObjectPtr(
    new geometry::Plane(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0, 0, 1), matR));

  scene->setSky(glm::vec3(0.0f));
  // build scene
  scene->add(sphereX);
  scene->add(sphereY);
  scene->add(sphereZ);
  scene->add(sphereLight);
  scene->add(plane1);
  scene->add(plane2);
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