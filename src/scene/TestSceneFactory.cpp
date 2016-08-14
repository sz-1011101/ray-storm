#include "scene/TestSceneFactory.h"
#include "geometry/Sphere.hpp"
#include "geometry/Plane.hpp"
#include "geometry/Rectangle.hpp"
#include "materials/Lambertian.hpp"
#include "materials/Mirror.hpp"
#include "materials/Phong.hpp"

using namespace ray_storm::scene;

ScenePtr TestSceneFactory::createDefaultScene()
{
  ScenePtr scene(new scene::Scene());

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
  geometry::ObjectPtr plane = geometry::ObjectPtr(
    new geometry::Plane(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0, 1, 0), matWhite));
  geometry::Rectangle::RectParams rectParams(glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), 2.0f, 1.0f);

  geometry::ObjectPtr rect = geometry::ObjectPtr(new geometry::Rectangle(rectParams, matR));

  scene->setSky(glm::vec3(0.0f));
  // build scene
  scene->add(sphereX);
  scene->add(sphereY);
  scene->add(sphereZ);
  scene->add(sphereLight);
  scene->add(plane);
  scene->add(rect);
  scene->finalize();

  return scene;
}

ScenePtr TestSceneFactory::createCornellBox()
{
  ScenePtr scene(new scene::Scene());

  // materials
  materials::AbstractMaterialPtr matLight(new materials::Lambertian(glm::vec3(0.0f), glm::vec3(100.0f)));
  materials::AbstractMaterialPtr matRed(new materials::Lambertian(glm::vec3(1.0f, 0.3f, 0.3f), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matGreen(new materials::Lambertian(glm::vec3(0.3f, 1.0f, 0.3f), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matBlue(new materials::Lambertian(glm::vec3(0.3f, 0.3f, 1.0f), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matWhite(new materials::Lambertian(glm::vec3(0.7f), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matYellow(new materials::Lambertian(glm::vec3(0.6f, 0.6f, 0.2f), glm::vec3(0.0f)));
  materials::AbstractMaterialPtr matMirror(new materials::Mirror(glm::vec3(1.0f)));
  materials::AbstractMaterialPtr matMetal1(new materials::Phong(glm::vec3(0.25f), glm::vec3(0.75f), 10.0f, glm::vec3(0.0f)));

  // cornell box walls

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
  geometry::Rectangle::RectParams lightRp(glm::vec3(-1.5f, 9.99f, -1.5f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 3.0f, 3.0f);
  geometry::ObjectPtr light = geometry::ObjectPtr(new geometry::Rectangle(lightRp, matLight));

  geometry::ObjectPtr sphere1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-2.0f, 2.0f, 1.0f), 2.0f, matMetal1));
  geometry::ObjectPtr sphere2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(2.0f, 2.0f, -1.0f), 2.0f, matMirror));
  // build scene
  scene->add(floor);
  scene->add(ceiling);
  scene->add(leftWall);
  scene->add(rightWall);
  scene->add(backWall);
  scene->add(frontWall);
  scene->add(light);
  scene->add(sphere1);
  scene->add(sphere2);
  scene->finalize();

  // TODO: implement
  return scene;
}