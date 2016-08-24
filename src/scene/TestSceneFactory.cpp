#include "scene/TestSceneFactory.h"
#include "geometry/Sphere.hpp"
#include "geometry/Rectangle.hpp"
#include "geometry/Box.hpp"
#include "materials/MaterialFactory.h"

using namespace ray_storm::scene;

ScenePtr TestSceneFactory::createCornellBox()
{
  ScenePtr scene = TestSceneFactory::buildBox();

  materials::MaterialPtr matWhite = materials::MaterialFactory::createLambertian(glm::vec3(0.75f));
  materials::MaterialPtr matGlass = materials::MaterialFactory::createGlass(glm::vec3(1.0f), 1.5f);
  materials::MaterialPtr matMirror = materials::MaterialFactory::createMirror(glm::vec3(1.0f));
  materials::MaterialPtr matMetal = materials::MaterialFactory::createMetal(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.25f, 0.1f), 55.0f);
  materials::MaterialPtr matDiffGlass = materials::MaterialFactory::createDiffuseGlass(glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(0.5f, 0.3f, 0.1f), 33.0f, 1.5f);

  // light
  geometry::ObjectPtr sphereLight1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-1, 10, 2), 0.2f, matWhite, glm::vec3(200.0f)));
  geometry::ObjectPtr sphereLight2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(2, 10, -1), 0.2f, matWhite, glm::vec3(200.0f)));

  geometry::ObjectPtr sphere1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-1, 1, 2), 1.0f, matDiffGlass));
  geometry::ObjectPtr sphere2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(3, 1, 2), 1.0f, matMetal));
  geometry::ObjectPtr sphere3 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(3, 4.5f, -2), 1.5f, matMirror));
  geometry::ObjectPtr sphere4 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-2, 2, -2), 2.0f, matGlass));

  geometry::ObjectPtr box = geometry::ObjectPtr(new geometry::Box(glm::vec3(1.5f, 0.0f, -3.5f), glm::vec3(3.0f), matDiffGlass));

  // add our components
  scene->add(sphereLight1);
  scene->add(sphereLight2);
  scene->add(sphere1);
  scene->add(sphere2);
  scene->add(sphere3);
  scene->add(sphere4);
  scene->add(box);
  scene->finalize();

  return scene;
}

ScenePtr TestSceneFactory::createReflectionTest()
{
  ScenePtr scene = TestSceneFactory::buildBigWall();
  materials::MaterialPtr matShiny1 = materials::MaterialFactory::createShiny(glm::vec3(0.2f), glm::vec3(0.8f), 30.0f, 1.5f);
  materials::MaterialPtr matShiny2 = materials::MaterialFactory::createShiny(glm::vec3(0.2f), glm::vec3(0.8f), 30.0f, 4.0f);
  materials::MaterialPtr matLambertian = materials::MaterialFactory::createLambertian(glm::vec3(0.2f));

  geometry::ObjectPtr sphere1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-5.0f, 5.0f, 0.0f), 1.0f, matShiny1));
  geometry::ObjectPtr sphere2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(0.0f, 5.0f, 0.0f), 1.0f, matShiny2));
  geometry::ObjectPtr sphere3 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(5.0f, 5.0f, 0.0f), 1.0f, matLambertian));

  scene->add(sphere1);
  scene->add(sphere2);
  scene->add(sphere3);
  scene->finalize();

  return scene;
}

ScenePtr TestSceneFactory::buildBox()
{
  ScenePtr box(new scene::Scene());
  // (some) colors stolen from http://www.kevinbeason.com/smallpt/
  materials::MaterialPtr matWhite = materials::MaterialFactory::createLambertian(glm::vec3(0.75f));
  materials::MaterialPtr matRed = materials::MaterialFactory::createLambertian(glm::vec3(0.75f, 0.25f, 0.25f));
  materials::MaterialPtr matBlue = materials::MaterialFactory::createLambertian(glm::vec3(0.25f, 0.25f, 0.75f));

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
  geometry::ObjectPtr backWall = geometry::ObjectPtr(new geometry::Rectangle(backWallRp, matWhite));

  // front wall
  geometry::Rectangle::RectParams frontWallRp(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f);
  geometry::ObjectPtr frontWall = geometry::ObjectPtr(new geometry::Rectangle(frontWallRp, matWhite));

  box->add(floor);
  box->add(ceiling);
  box->add(leftWall);
  box->add(rightWall);
  box->add(backWall);
  box->add(frontWall);

  return box;
}

ScenePtr TestSceneFactory::buildBigWall()
{
  ScenePtr wall(new scene::Scene());

  materials::MaterialPtr matWhite = materials::MaterialFactory::createLambertian(glm::vec3(0.75f));

  geometry::Rectangle::RectParams wallRp(glm::vec3(-50.0f, 50.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 100.0f, 100.0f);
  geometry::ObjectPtr _wall = geometry::ObjectPtr(new geometry::Rectangle(wallRp, matWhite, glm::vec3(1.0f)));

  wall->add(_wall);

  return wall;
}