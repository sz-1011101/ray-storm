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
  // at 435 nm (green)
  materials::MaterialPtr matMetal1 = materials::MaterialFactory::createMetalFresnel(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.6f, 0.2f), 55.0f, 1.4523, 1.8009);
  materials::MaterialPtr matCoating = materials::MaterialFactory::createShiny(glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), 500.0f);
  materials::MaterialPtr matDiffGlass = materials::MaterialFactory::createDiffuseGlass(glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(0.5f, 0.3f, 0.1f), 33.0f, 1.5f);

  // light
  geometry::ObjectPtr sphereLight1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-1, 10, 2), 0.2f, matWhite, glm::vec3(200.0f)));
  geometry::ObjectPtr sphereLight2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(2, 8, -1), 0.1f, matWhite, glm::vec3(200.0f)));

  geometry::ObjectPtr sphere1 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-1, 1, 2), 1.0f, matDiffGlass));
  geometry::ObjectPtr sphere2 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(3, 1, 2), 1.0f, matCoating));
  geometry::ObjectPtr sphere3 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(3, 4.5f, -2), 1.5f, matMirror));
  geometry::ObjectPtr sphere4 = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(-2, 2, -2), 2.0f, matGlass));

  geometry::ObjectPtr box = geometry::ObjectPtr(new geometry::Box(glm::vec3(1.5f, 0.0f, -3.5f), glm::vec3(3.0f), matMetal1));

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
  materials::MaterialPtr matShiny1 = materials::MaterialFactory::createShiny(glm::vec3(0.2f), glm::vec3(0.8f), 30.0f, 2.0f);
  materials::MaterialPtr matShiny2 = materials::MaterialFactory::MaterialFactory::createMetalFresnel(glm::vec3(0.2f), glm::vec3(0.8f), 30.0f, 2.0f, 0.1f);
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

ScenePtr TestSceneFactory::createSolarSystem()
{
  ScenePtr scene(new scene::Scene());

  materials::MaterialPtr matBlack = materials::MaterialFactory::createLambertian(glm::vec3(0.0f));
  materials::MaterialPtr matMercury = materials::MaterialFactory::createMetal(glm::vec3(0.25), glm::vec3(0.2), 35.0f);
  materials::MaterialPtr matVenus = materials::MaterialFactory::createLambertian(glm::vec3(0.65f, 0.3f, 0.15f));
  materials::MaterialPtr matEarth = materials::MaterialFactory::createShiny(glm::vec3(0.1, 0.6, 0.9), glm::vec3(0.8, 0.8, 1.0), 150.0f);
  materials::MaterialPtr matMars = materials::MaterialFactory::createLambertian(glm::vec3(0.8, 0.3, 0.3));
  materials::MaterialPtr matJupiter = materials::MaterialFactory::createLambertian(glm::vec3(0.8, 0.7, 0.1));
  materials::MaterialPtr matSaturn = materials::MaterialFactory::createLambertian(glm::vec3(0.65, 0.6, 0.1));
  materials::MaterialPtr matUranus = materials::MaterialFactory::createLambertian(glm::vec3(0.75, 0.8, 1.0));
  materials::MaterialPtr matNeptune = materials::MaterialFactory::createLambertian(glm::vec3(0.2, 0.1, 8.0));

  // objects scale correct, distance not
  // unit length = 10000 km
  const float x = -95.0f;
  const float z = -50.0f;
  geometry::ObjectPtr sun = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x, 0, z), 69.57f, matBlack, glm::vec3(1.0f)));

  geometry::ObjectPtr mercury = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 73.50f, 0, z), 0.2439f, matMercury));

  geometry::ObjectPtr venus = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 75.50f, 0, z), 0.60518f, matVenus));

  geometry::ObjectPtr earth = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 77.50f, 0, z), 0.6371f, matEarth));

  geometry::ObjectPtr mars = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 79.50f, 0, z), 0.33895f, matMars));

  geometry::ObjectPtr jupiter = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 90.0f, 0, z), 6.9911f, matJupiter));

  geometry::ObjectPtr saturn = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 110.0f, 0, z), 5.8232f, matSaturn));

  geometry::ObjectPtr uranus = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 125.0f, 0, z), 2.5362f, matUranus));

  geometry::ObjectPtr neptune = geometry::ObjectPtr(new geometry::Sphere(glm::vec3(x + 135.0f, 0, z), 2.4622f, matNeptune));

  scene->add(sun);
  scene->add(mercury);
  scene->add(venus);
  scene->add(earth);
  scene->add(mars);
  scene->add(jupiter);
  scene->add(saturn);
  scene->add(uranus);
  scene->add(neptune);
  scene->finalize();
  // sorry pluto :(

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