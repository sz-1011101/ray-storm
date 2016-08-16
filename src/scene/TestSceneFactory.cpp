#include "scene/TestSceneFactory.h"
#include "geometry/Sphere.hpp"
#include "geometry/Plane.hpp"
#include "geometry/Rectangle.hpp"
#include "materials/Lambertian.hpp"
#include "materials/Mirror.hpp"
#include "materials/Phong.hpp"
#include "materials/Glass.hpp"
#include "materials/ScatteringGlass.hpp"

using namespace ray_storm::scene;

ScenePtr TestSceneFactory::createCornellBox()
{
  ScenePtr scene(new scene::Scene());

  // colors stolen from http://www.kevinbeason.com/smallpt/
  materials::AbstractBRDFPtr lambertianWhite(new materials::Lambertian(glm::vec3(0.75)));
  materials::AbstractBRDFPtr lambertianRed(new materials::Lambertian(glm::vec3(0.75f,0.25f,0.25f)));
  materials::AbstractBRDFPtr lambertianBlue(new materials::Lambertian(glm::vec3(0.25f, 0.25f, 0.75f)));
  materials::AbstractBRDFPtr lambertianYellow(new materials::Lambertian(glm::vec3(0.75f, 0.75f, 0.25f)));
  materials::AbstractBRDFPtr lambertianGreen(new materials::Lambertian(glm::vec3(0.25f, 0.75f, 0.25f)));
  materials::AbstractBRDFPtr phongMetal1(new materials::Phong(glm::vec3(0.4f), glm::vec3(0.6f), 5.0f));
  materials::AbstractBRDFPtr phongMetal2(new materials::Phong(glm::vec3(0.8f), glm::vec3(0.2f), 50.0f));
  materials::AbstractBRDFPtr phongWhite(new materials::Phong(glm::vec3(0.5f), glm::vec3(0.5f), 150.0f));

  materials::AbstractBTDFPtr transparent(new materials::Glass(glm::vec3(1.0f)));
  materials::AbstractBTDFPtr milky(new materials::ScatteringGlass(glm::vec3(1.0f), 150.0f));

  // materials
  materials::MaterialPtr matLight(new materials::Material(lambertianWhite, 3.0f, glm::vec3(50.0f, 50.0f, 50.0f)));
  materials::MaterialPtr matWhite(new materials::Material(lambertianWhite, 3.0f));
  materials::MaterialPtr matRed(new materials::Material(lambertianRed, 3.0f));
  materials::MaterialPtr matBlue(new materials::Material(lambertianBlue, 3.0f));
  materials::MaterialPtr matYellow(new materials::Material(lambertianYellow, 3.0f));
  materials::MaterialPtr matGreen(new materials::Material(lambertianGreen, 3.0f));
  materials::MaterialPtr matMetal1(new materials::Material(phongMetal1, 1.3f));
  materials::MaterialPtr matMetal2(new materials::Material(phongMetal2, 1.3f));
  materials::MaterialPtr matGlass(new materials::Material(transparent, 1.5f));
  materials::MaterialPtr matMilkyGlass(new materials::Material(phongWhite, milky, 1.35f));

  // cornell box walls, 

  // floor
  geometry::Rectangle::RectParams floorRp(glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 10.0f, 15.0f);
  geometry::ObjectPtr floor = geometry::ObjectPtr(new geometry::Rectangle(floorRp, matWhite));

  // ceiling
  geometry::Rectangle::RectParams ceilingRp(glm::vec3(-5.0f, 10.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 10.0f, 15.0f);
  geometry::ObjectPtr ceiling = geometry::ObjectPtr(new geometry::Rectangle(ceilingRp, matWhite));

  // left wall
  geometry::Rectangle::RectParams leftWallRp(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), 15.0f, 10.0f);
  geometry::ObjectPtr leftWall = geometry::ObjectPtr(new geometry::Rectangle(leftWallRp, matRed));

  // right wall
  geometry::Rectangle::RectParams rightWallRp(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), 15.0f, 10.0f);
  geometry::ObjectPtr rightWall = geometry::ObjectPtr(new geometry::Rectangle(rightWallRp, matBlue));

  // back wall
  geometry::Rectangle::RectParams backWallRp(glm::vec3(-5.0f, 10.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f);
  geometry::ObjectPtr backWall = geometry::ObjectPtr(new geometry::Rectangle(backWallRp, matYellow));

  // front wall
  geometry::Rectangle::RectParams frontWallRp(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f);
  geometry::ObjectPtr frontWall = geometry::ObjectPtr(new geometry::Rectangle(frontWallRp, matWhite));

  // light
  geometry::ObjectPtr sphereLight = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(0, 8, 0), 0.5f, matLight));

  geometry::ObjectPtr sphere1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-2.0f, 1.5f, 1.0f), 1.5f, matWhite));
  geometry::ObjectPtr sphere2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(1.0f, 1.5f, -3.0f), 1.5f, matWhite));
  // build scene
  scene->add(floor);
  scene->add(ceiling);
  scene->add(leftWall);
  scene->add(rightWall);
  scene->add(backWall);
  scene->add(frontWall);
  scene->add(sphereLight);
  scene->add(sphere1);
  scene->add(sphere2);
  scene->finalize();

  // TODO: implement
  return scene;
}