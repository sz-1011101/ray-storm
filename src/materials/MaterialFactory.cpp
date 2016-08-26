#include "materials/MaterialFactory.h"

#include "materials/Lambertian.hpp"
#include "materials/Mirror.hpp"
#include "materials/Phong.hpp"
#include "materials/Glass.hpp"
#include "materials/Material.hpp"
#include "materials/ScatteringGlass.hpp"

using namespace ray_storm::materials;

ray_storm::materials::MaterialPtr MaterialFactory::createLambertian(const glm::vec3 &color)
{
  materials::AbstractBRDFPtr lambertianBRDF(new materials::Lambertian(color));
  return MaterialPtr(new Material(lambertianBRDF));
}

ray_storm::materials::MaterialPtr MaterialFactory::createMetal(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness
)
{
  materials::AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  MaterialPtr metal = MaterialPtr(new Material(phongBRDF));
  metal->setUseFresnel(false);
  metal->setConstReflectance(1.0f);
  return metal;
}

ray_storm::materials::MaterialPtr MaterialFactory::createMirror
(
  const glm::vec3 &color
)
{
  materials::AbstractBRDFPtr mirrorBRDF(new materials::Mirror(color));
  MaterialPtr mirror = MaterialPtr(new Material(mirrorBRDF));
  mirror->setUseFresnel(false);
  mirror->setConstReflectance(1.0f);
  return mirror;
}

ray_storm::materials::MaterialPtr MaterialFactory::createGlass(
  const glm::vec3 &color,
  float indexOfRefraction
)
{
  materials::AbstractBTDFPtr glassBTDF(new materials::Glass(color, indexOfRefraction));
  materials::AbstractBRDFPtr mirrorBRDF(new materials::Mirror(color));
  MaterialPtr glass = MaterialPtr(new Material(mirrorBRDF, glassBTDF));
  glass->setIndexOfRefraction(indexOfRefraction);
  glass->setUseFresnel(true);
  return glass;
}

ray_storm::materials::MaterialPtr MaterialFactory::createDiffuseGlass(
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float scattering, 
        float indexOfRefraction
)
{
  materials::AbstractBTDFPtr scatteringGlassBTDF(new materials::ScatteringGlass(diffuse, specular, scattering, indexOfRefraction));
  MaterialPtr sGlass = MaterialPtr(new Material(scatteringGlassBTDF));
  sGlass->setIndexOfRefraction(indexOfRefraction);
  sGlass->setUseFresnel(false);
  sGlass->setConstReflectance(0.0f);
  return sGlass;
}

ray_storm::materials::MaterialPtr MaterialFactory::createShiny(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness,
  float indexOfRefraction
)
{
  materials::AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  MaterialPtr shiny = MaterialPtr(new Material(phongBRDF));
  shiny->setIndexOfRefraction(indexOfRefraction);
  shiny->setUseFresnel(true);
  return shiny;
}

ray_storm::materials::MaterialPtr MaterialFactory::createShiny(
  const glm::vec3 &diffuse,
  const glm::vec3 &specular,
  float shinyness
)
{
  materials::AbstractBRDFPtr phongBRDF(new materials::Phong(diffuse, specular, shinyness));
  MaterialPtr shiny = MaterialPtr(new Material(phongBRDF));
  shiny->setUseFresnel(false);
  shiny->setConstReflectance(1.0f);
  return shiny;
}

ray_storm::materials::MaterialPtr MaterialFactory::createCombined(
  AbstractBRDFPtr &brdf,
  AbstractBTDFPtr &btdf,
  float constReflectance
)
{
  MaterialPtr combined = MaterialPtr(new Material(brdf, btdf, constReflectance));
  combined->setIndexOfRefraction(btdf->getIndexOfRefraction());
  combined->setUseFresnel(false);
  return combined;
}