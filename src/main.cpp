#include "scene/Scene.h"
#include "geometry/Sphere.hpp"
#include "camera/PinholeCamera.h"
#include "materials/Lambertian.hpp"
#include "renderer/PathTracer.h"

int main(int argc, char* argv[])
{

  using namespace ray_storm;

  scene::Scene scene;

  camera::PinholeCamera camera(glm::vec3(5,5,5), glm::vec3(0,0,0), glm::vec3(0, 1, 0), 1.0f, 90.0f);
  materials::AbstractMaterialPtr mat1(new materials::Lambertian(glm::vec3(0.25), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr mat2(new materials::Lambertian(glm::vec3(0.25), glm::vec3(1.0f)));

  geometry::ObjectPtr sphere1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, mat1));
  geometry::ObjectPtr sphere2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(0.0f, 2.0f, 0.0f), 1.0f, mat2));
  scene.add(sphere1);
  scene.add(sphere2);
  
  return 0;
}