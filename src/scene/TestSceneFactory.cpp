#include "scene/TestSceneFactory.h"
#include "objects/Sphere.hpp"
#include "objects/Rectangle.hpp"
#include "objects/Box.hpp"
#include "objects/ObjectFactory.h"
#include "textures/TextureFactory.h"
#include "materials/MaterialFactory.h"
#include "scene/ConstantSky.hpp"
#include "scene/SunSky.hpp"
#include "geometry/SurfaceNormalModifierFactory.h"

using namespace ray_storm::scene;

ScenePtr TestSceneFactory::createCornellBox(bool naturalLighting, bool lightSources)
{
  ScenePtr scene = TestSceneFactory::buildBox();

  materials::AbstractSVBxDFPtr matWhite = materials::MaterialFactory::createLambertian(glm::vec3(0.75f));
  materials::AbstractSVBxDFPtr matGlass = materials::MaterialFactory::createGlass(glm::vec3(1.0f), 1.5f);
  materials::AbstractSVBxDFPtr matMirror = materials::MaterialFactory::createMirror(glm::vec3(1.0f));
  // values from http://refractiveindex.info/
  // gold (Au) at 435 nm (green)
  //materials::AbstractSVBxDFPtr matMetal1 = materials::MaterialFactory::createMetalFresnel(
  // glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.6f, 0.2f), 55.0f, 1.4523f, 1.8009f
  //);

  // aluminium (Al) at 435 nm (green)
  //materials::AbstractSVBxDFPtr matMetal2 = materials::MaterialFactory::createMetalFresnel(
  //  glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.9f, 0.9, 0.9f), 85.0f, 0.56884f, 5.1289f
  //);
  
  //const glm::vec2 matCoatingFreq(21.0f, 10.0f);
  materials::AbstractSVBxDFPtr matMarble = materials::MaterialFactory::createShiny(
    textures::TextureFactory::createTurbulence2DTexture<glm::vec3>(6, 1.0f, 2.0f, 2.5f, glm::vec3(1.0f), glm::vec3(0.5f, 0.3f, 0.1f)),
    textures::TextureFactory::createConstant2DTexture<glm::vec3>(glm::vec3(0.25f, 0.1f, 0.1f)),
    textures::TextureFactory::createTurbulence2DTexture<float>(6, 1.0f, 2.0f, 5.0f, 15.0f, 55.0f)
  );
  materials::AbstractSVBxDFPtr matMarble2 = materials::MaterialFactory::createShiny(
    textures::TextureFactory::createTurbulence2DTexture<glm::vec3>(3, 1.0f, 2.0f, 10.0f, glm::vec3(0.8f, 0.9f, 0.9f), glm::vec3(0.05f, 0.1f, 0.05f)),
    textures::TextureFactory::createConstant2DTexture<glm::vec3>(glm::vec3(0.0f, 0.2f, 0.1f)),
    textures::TextureFactory::createTurbulence2DTexture<float>(5, 1.0f, 2.0f, 10.0f, 15.0f, 105.0f)
  );
  geometry::AbstractSurfaceNormalModifierPtr bumpMap1 = geometry::SurfaceNormalModifierFactory::createTextureBumpMap(
    textures::TextureFactory::createTurbulence2DTexture<float>(5, 1.0f, 2.0f, 2.0f, 0.0f, 1.0f)
  );
  geometry::AbstractSurfaceNormalModifierPtr bumpMap2 = geometry::SurfaceNormalModifierFactory::createTextureBumpMap(
    textures::TextureFactory::createTurbulence2DTexture<float>(2, 1.0f, 2.0f, 10.0f, 0.0f, 1.0f)
  );
  //materials::AbstractSVBxDFPtr matDiffGlass = materials::MaterialFactory::createDiffuseGlass(glm::vec3(0.3f, 0.1f, 0.2f), glm::vec3(0.5f, 0.3f, 0.8f), 55.0f, 1.5f);

  if (lightSources)
  {
    // ceiling light
    const objects::Rectangle::RectParams ceilLightRp(glm::vec3(-1.0f, 9.9f, -1.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 2.0f, 2.0f);
    objects::EmitterPtr ceilLight = objects::ObjectFactory::createRectangle(
      ceilLightRp,
      matWhite,
      glm::vec3(20.0f)
    );
    scene->add(ceilLight);
  }

  if (naturalLighting)
  {
    const glm::vec3 skyColor = glm::vec3(0.6f, 0.3f, 0.3f);
    const glm::vec3 sunColor = glm::vec3(1.0f);
    const glm::vec3 sunDir = glm::vec3(0.0f, -0.2f, -1.0f);
    scene->setSky(SunSkyPtr(new SunSky(skyColor, sunColor, sunDir, 650.0f)));
  }

  objects::EmitterPtr sphere1 = objects::ObjectFactory::createSphere(glm::vec3(-3, 0.75, 2), 1.0f, matWhite);
  objects::EmitterPtr sphere2 = objects::ObjectFactory::createSphere(glm::vec3(0, 1.0, 2), 1.0f, matWhite);
  objects::EmitterPtr sphere3 = objects::ObjectFactory::createSphere(glm::vec3(3, 4.5f, -2), 1.5f, matWhite);
  objects::EmitterPtr sphere4 = objects::ObjectFactory::createSphere(glm::vec3(-2, 2, -2), 2.0f, matWhite);
  objects::EmitterPtr sphere5 = objects::ObjectFactory::createSphere(glm::vec3(2.5f, 1.0, 2), 1.0f, matWhite);

  objects::ReflectorPtr sphereUnion1 = objects::ObjectFactory::createSphereUnion(glm::vec3(-2.5, 6.5, -2.5), matWhite);

  objects::ReflectorPtr box = objects::ObjectFactory::createBox(glm::vec3(1.5f, 0.0f, -3.5f), glm::vec3(3.0f), matWhite);

  sphere4->setSurfaceNormalModifier(bumpMap1);
  sphere2->setSurfaceNormalModifier(bumpMap1);
  sphere3->setSurfaceNormalModifier(bumpMap2);
  // add our components
  scene->add(sphere1);
  scene->add(sphere2);
  scene->add(sphere3);
  scene->add(sphere4);
  scene->add(sphere5);
  scene->add(sphereUnion1);
  scene->add(box);
  scene->finalize();

  return scene;
}


ScenePtr TestSceneFactory::buildBox()
{
  ScenePtr box(new scene::Scene());
  // (some) colors stolen from http://www.kevinbeason.com/smallpt/
  materials::AbstractSVBxDFPtr matWhite = materials::MaterialFactory::createLambertian(glm::vec3(0.75f));
  materials::AbstractSVBxDFPtr matRed = materials::MaterialFactory::createLambertian(glm::vec3(0.75f, 0.25f, 0.25f));
  materials::AbstractSVBxDFPtr matBlue = materials::MaterialFactory::createLambertian(glm::vec3(0.25f, 0.25f, 0.75f));

  materials::AbstractSVBxDFPtr matRing = materials::MaterialFactory::createLambertian(
    textures::TextureFactory::createRing2DTexture(glm::vec3(0.18f, 0.05f, 0.01f), glm::vec3(0.6f, 0.26f, 0.11f))
  );
  // floor
  objects::Rectangle::RectParams floorRp(glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 10.0f, 15.0f);
  objects::EmitterPtr floor = objects::ObjectFactory::createRectangle(floorRp, matRing);

  // ceiling
  objects::Rectangle::RectParams ceilingRp(glm::vec3(-5.0f, 10.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 10.0f, 15.0f);
  objects::EmitterPtr ceiling = objects::ObjectFactory::createRectangle(ceilingRp, matWhite);

  // left wall
  objects::Rectangle::RectParams leftWallRp(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), 15.0f, 10.0f);
  objects::EmitterPtr leftWall = objects::ObjectFactory::createRectangle(leftWallRp, matRed);

  // right wall
  objects::Rectangle::RectParams rightWallRp(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), 15.0f, 10.0f);
  objects::EmitterPtr rightWall = objects::ObjectFactory::createRectangle(rightWallRp, matBlue);

  // back wall
  objects::Rectangle::RectParams backWallRp(glm::vec3(-5.0f, 10.0f, -5.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f);
  objects::EmitterPtr backWall = objects::ObjectFactory::createRectangle(backWallRp, matWhite);

  // front wall
  //objects::Rectangle::RectParams frontWallRp(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f);
  //objects::EmitterPtr frontWall = objects::ObjectFactory::createRectangle(frontWallRp, matWhite);

  box->add(floor);
  box->add(ceiling);
  box->add(leftWall);
  box->add(rightWall);
  box->add(backWall);
  //box->add(frontWall);

  return box;
}