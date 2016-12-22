#include "materials/MaterialFactory.h"

#include "materials/Lambertian.hpp"
#include "materials/Mirror.hpp"
#include "materials/Phong.hpp"
#include "materials/Glass.hpp"
#include "materials/Material.hpp"
#include "materials/ScatteringGlass.hpp"
#include "materials/DielectricFresnel.hpp"
#include "materials/ConstantReflectivity.hpp"
#include "materials/ConductorFresnel.hpp"

using namespace ray_storm::materials;

ray_storm::materials::MaterialPtr MaterialFactory::createLambertian(const glm::vec3 &color)
{
  AbstractBRDFPtr lambertianBRDF(new materials::Lambertian(color));
  return MaterialPtr(new Material(lambertianBRDF));
}

MaterialPtr MaterialFactory::createLambertian(const textures::Abstract2DTexturePtr<glm::vec3> &color)
{
  AbstractBRDFPtr lambertianBRDF(new materials::Lambertian(color));
  return MaterialPtr(new Material(lambertianBRDF));
}

ray_storm::materials::MaterialPtr MaterialFactory::createMetal(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness
)
{
  AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  return MaterialPtr(new Material(phongBRDF));
}

ray_storm::materials::MaterialPtr MaterialFactory::createMetalFresnel(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness,
  float indexOfRefraction,
  float absorption
)
{
  AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  AbstractReflectivityPtr conductor(new materials::ConductorFresnel(absorption));
  return MaterialPtr(new Material(phongBRDF, indexOfRefraction, conductor));
}

ray_storm::materials::MaterialPtr MaterialFactory::createMirror
(
  const glm::vec3 &color
)
{
  AbstractBRDFPtr mirrorBRDF(new materials::Mirror(color));
  return MaterialPtr(new Material(mirrorBRDF));
}

MaterialPtr MaterialFactory::createMirror(
  const textures::Abstract2DTexturePtr<glm::vec3> &color
)
{
  AbstractBRDFPtr mirrorBRDF(new materials::Mirror(color));
  return MaterialPtr(new Material(mirrorBRDF));
}

ray_storm::materials::MaterialPtr MaterialFactory::createGlass(
  const glm::vec3 &color,
  float indexOfRefraction
)
{
  AbstractBTDFPtr glassBTDF(new materials::Glass(color, indexOfRefraction));
  AbstractBRDFPtr mirrorBRDF(new materials::Mirror(color));
  AbstractReflectivityPtr dielectric(new materials::DielectricFresnel());
  return MaterialPtr(new Material(mirrorBRDF, glassBTDF, dielectric));
}

ray_storm::materials::MaterialPtr MaterialFactory::createDiffuseGlass(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float scattering, 
        float indexOfRefraction
)
{
  AbstractBTDFPtr scatteringGlassBTDF(new materials::ScatteringGlass(diffuse, specular, scattering, indexOfRefraction));
  return MaterialPtr(new Material(scatteringGlassBTDF));
}

ray_storm::materials::MaterialPtr MaterialFactory::createShiny(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness,
  float indexOfRefraction
)
{
  AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  AbstractReflectivityPtr dielectric(new materials::DielectricFresnel());
  return MaterialPtr(new Material(phongBRDF, indexOfRefraction, dielectric));
}

ray_storm::materials::MaterialPtr MaterialFactory::createShiny(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness
)
{
  AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  return MaterialPtr(new Material(phongBRDF));
}

ray_storm::materials::MaterialPtr MaterialFactory::createCombined(
  const AbstractBRDFPtr &brdf,
  const AbstractBTDFPtr &btdf,
  float constReflectance
)
{
  AbstractReflectivityPtr constantRefl(new materials::ConstantReflectivity(constReflectance));
  return MaterialPtr(new Material(brdf, btdf, constantRefl));
}