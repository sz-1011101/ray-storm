#include "materials/MaterialFactory.h"

#include "materials/Lambertian.hpp"
#include "materials/Mirror.hpp"
#include "materials/Phong.hpp"
#include "materials/Glass.hpp"
#include "materials/DielectricFresnel.hpp"
#include "materials/ConstantReflectivity.hpp"
#include "materials/ConductorFresnel.hpp"
#include "materials/CombinedBSDF.hpp"
#include "materials/ReflectivityBRDF.hpp"

using namespace ray_storm::materials;

AbstractSVBxDFPtr MaterialFactory::createLambertian(const glm::vec3 &color)
{
  return AbstractBRDFPtr(new materials::Lambertian(color));
}

AbstractSVBxDFPtr MaterialFactory::createLambertian(const textures::Abstract2DTexturePtr<glm::vec3> &color)
{
  return AbstractBRDFPtr(new materials::Lambertian(color));
}

AbstractSVBxDFPtr MaterialFactory::createMetal(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness
)
{
  return AbstractBRDFPtr(new materials::Phong(diffuse, specular, shinyness));
}

AbstractSVBxDFPtr  MaterialFactory::createMetalFresnel(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness,
  float indexOfRefraction,
  float absorption
)
{
  AbstractBRDFPtr phong(new materials::Phong(diffuse, specular, shinyness));
  AbstractReflectivityPtr conductor(new materials::ConductorFresnel(indexOfRefraction, absorption));
  return AbstractBRDFPtr(new ReflectivityBRDF(phong, conductor));
}

AbstractSVBxDFPtr MaterialFactory::createMirror
(
  const glm::vec3 &color
)
{
  return AbstractBRDFPtr(new materials::Mirror(color));
}

AbstractSVBxDFPtr MaterialFactory::createMirror(
  const textures::Abstract2DTexturePtr<glm::vec3> &color
)
{
  return AbstractBRDFPtr(new materials::Mirror(color));
}

AbstractSVBxDFPtr MaterialFactory::createGlass(
  const glm::vec3 &color,
  float indexOfRefraction
)
{
  AbstractBTDFPtr glassBTDF(new materials::Glass(color, indexOfRefraction));
  AbstractBRDFPtr mirrorBRDF(new materials::Mirror(color));
  AbstractReflectivityPtr dielectric(new materials::DielectricFresnel(indexOfRefraction));
  return AbstractBSDFPtr(new CombinedBSDF(mirrorBRDF, glassBTDF, dielectric));
}

AbstractSVBxDFPtr MaterialFactory::createShiny(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness
)
{
  return AbstractBRDFPtr(new materials::Phong(diffuse, specular, shinyness));
}

AbstractSVBxDFPtr MaterialFactory::createShiny(
  const textures::Abstract2DTexturePtr<glm::vec3> &diffuse,
  const textures::Abstract2DTexturePtr<glm::vec3> &specular,
  const textures::Abstract2DTexturePtr<float> &shinyness
)
{
  return AbstractBRDFPtr(new materials::Phong(diffuse, specular, shinyness));
}

AbstractSVBxDFPtr MaterialFactory::createShinyFresnel(
  const textures::Abstract2DTexturePtr<glm::vec3> &diffuse,
  const textures::Abstract2DTexturePtr<glm::vec3> &specular,
  const textures::Abstract2DTexturePtr<float> &shinyness,
  float indexOfRefraction
)
{
  AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  AbstractReflectivityPtr dielectric(new materials::DielectricFresnel(indexOfRefraction));
  return AbstractBRDFPtr(new ReflectivityBRDF(phongBRDF, dielectric));
}

AbstractSVBxDFPtr MaterialFactory::createCombined(
  const AbstractBRDFPtr &brdf,
  const AbstractBTDFPtr &btdf,
  float constReflectance
)
{
  AbstractReflectivityPtr constantRefl(new materials::ConstantReflectivity(constReflectance));
  return AbstractBSDFPtr(new CombinedBSDF(brdf, btdf, constantRefl));
}