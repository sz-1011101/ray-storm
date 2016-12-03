#include "scene/TestSceneFactory.h"
#include "objects/Sphere.hpp"
#include "objects/Rectangle.hpp"
#include "objects/Box.hpp"
#include "materials/MaterialFactory.h"
#include "scene/ConstantSky.hpp"
#include "scene/SunSky.hpp"

using namespace ray_storm::scene;

ScenePtr TestSceneFactory::createCornellBox(bool naturalLighting, bool lightSources)
{
  ScenePtr scene = TestSceneFactory::buildBox();

  materials::MaterialPtr matWhite = materials::MaterialFactory::createLambertian(glm::vec3(0.75f));
  materials::MaterialPtr matGlass = materials::MaterialFactory::createGlass(glm::vec3(1.0f), 1.5f);
  materials::MaterialPtr matMirror = materials::MaterialFactory::createMirror(glm::vec3(1.0f));
  // at 435 nm (green)
  materials::MaterialPtr matMetal1 = materials::MaterialFactory::createMetalFresnel(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.6f, 0.2f), 55.0f, 1.4523, 1.8009);
  materials::MaterialPtr matCoating = materials::MaterialFactory::createShiny(glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), 100.0f);
  materials::MaterialPtr matDiffGlass = materials::MaterialFactory::createDiffuseGlass(glm::vec3(0.3f, 0.1f, 0.1f), glm::vec3(0.5f, 0.3f, 0.1f), 100.0f, 1.5f);

  if (lightSources)
  {
    //objects::EmitterPtr sphereLight1 = objects::EmitterPtr(new objects::Sphere(glm::vec3(3, 8.5f, 2), 0.2f, matWhite, glm::vec3(100.0f)));
    //scene->add(sphereLight1);
    // ceiling light
    objects::Rectangle::RectParams ceilLightRp(glm::vec3(-1.0f, 9.9f, -1.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 2.0f, 2.0f);
    objects::EmitterPtr ceilLight = objects::EmitterPtr(new objects::Rectangle(ceilLightRp, matWhite, glm::vec3(20.0f)));
    scene->add(ceilLight);
  }

  if (naturalLighting)
  {
    const glm::vec3 skyColor = glm::vec3(0.6f, 0.3f, 0.3f);
    const glm::vec3 sunColor = glm::vec3(1.0f);
    const glm::vec3 sunDir = glm::vec3(0.0f, -0.2f, -1.0f);
    scene->setSky(SunSkyPtr(new SunSky(skyColor, sunColor, sunDir, 650.0f)));
  }

  objects::EmitterPtr sphere1 = objects::EmitterPtr(new objects::Sphere(glm::vec3(-1, 1, 2), 1.0f, matDiffGlass));
  objects::EmitterPtr sphere2 = objects::EmitterPtr(new objects::Sphere(glm::vec3(3, 2, 2), 1.0f, matGlass));
  objects::EmitterPtr sphere3 = objects::EmitterPtr(new objects::Sphere(glm::vec3(3, 4.5f, -2), 1.5f, matCoating));
  objects::EmitterPtr sphere4 = objects::EmitterPtr(new objects::Sphere(glm::vec3(-2, 2, -2), 2.0f, matMirror));

  objects::ReflectorPtr box = objects::ReflectorPtr(new objects::Box(glm::vec3(1.5f, 0.0f, -3.5f), glm::vec3(3.0f), matMirror));

  // add our components
  //scene->add(sphere1);
  scene->add(sphere2);
  //scene->add(sphere3);
  //scene->add(sphere4);
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

  objects::EmitterPtr sphere1 = objects::EmitterPtr(new objects::Sphere(glm::vec3(-5.0f, 5.0f, 0.0f), 1.0f, matShiny1));
  objects::EmitterPtr sphere2 = objects::EmitterPtr(new objects::Sphere(glm::vec3(0.0f, 5.0f, 0.0f), 1.0f, matShiny2));
  objects::EmitterPtr sphere3 = objects::EmitterPtr(new objects::Sphere(glm::vec3(5.0f, 5.0f, 0.0f), 1.0f, matLambertian));

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
  objects::Rectangle::RectParams floorRp(glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 10.0f, 15.0f);
  objects::EmitterPtr floor = objects::EmitterPtr(new objects::Rectangle(floorRp, matWhite));

  // ceiling
  objects::Rectangle::RectParams ceilingRp(glm::vec3(-5.0f, 10.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 10.0f, 15.0f);
  objects::EmitterPtr ceiling = objects::EmitterPtr(new objects::Rectangle(ceilingRp, matWhite));

  // left wall
  objects::Rectangle::RectParams leftWallRp(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), 15.0f, 10.0f);
  objects::EmitterPtr leftWall = objects::EmitterPtr(new objects::Rectangle(leftWallRp, matRed));

  // right wall
  objects::Rectangle::RectParams rightWallRp(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), 15.0f, 10.0f);
  objects::EmitterPtr rightWall = objects::EmitterPtr(new objects::Rectangle(rightWallRp, matBlue));

  // back wall
  objects::Rectangle::RectParams backWallRp(glm::vec3(-5.0f, 10.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f);
  objects::EmitterPtr backWall = objects::EmitterPtr(new objects::Rectangle(backWallRp, matWhite));

  // front wall
  //objects::Rectangle::RectParams frontWallRp(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f);
  //objects::ObjectPtr frontWall = objects::ObjectPtr(new objects::Rectangle(frontWallRp, matWhite));

  box->add(floor);
  box->add(ceiling);
  box->add(leftWall);
  box->add(rightWall);
  box->add(backWall);
  //box->add(frontWall);

  return box;
}

ScenePtr TestSceneFactory::buildBigWall()
{
  ScenePtr wall(new scene::Scene());

  materials::MaterialPtr matWhite = materials::MaterialFactory::createLambertian(glm::vec3(0.75f));

  objects::Rectangle::RectParams wallRp(glm::vec3(-50.0f, 50.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 100.0f, 100.0f);
  objects::EmitterPtr _wall = objects::EmitterPtr(new objects::Rectangle(wallRp, matWhite, glm::vec3(1.0f)));

  wall->add(_wall);

  return wall;
}