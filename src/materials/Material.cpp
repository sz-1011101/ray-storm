#include "materials/Material.h"

using namespace ray_storm::materials;

Material::Material(
  AbstractBRDFPtr &brdf,
  AbstractBTDFPtr &btdf,
  float reflectance,
  float indexOfRefraction,
  const glm::vec3 &emittance
) : brdf(brdf), btdf(btdf)
{
  this->reflectance = reflectance;
  this->indexOfRefraction = indexOfRefraction;
  this->emittance = emittance;
}

Material::Material(
  AbstractBRDFPtr &brdf,
  const glm::vec3 &emittance
  ) : brdf(brdf)
{
  this->emittance = emittance;
  this->reflectance = 1.0f;
  this->indexOfRefraction = 1.0f;
}

Material::Material(
  AbstractBTDFPtr &btdf,
  float indexOfRefraction,
  const glm::vec3 &emittance
) : btdf(btdf)
{
  this->indexOfRefraction = indexOfRefraction;
  this->emittance = emittance;

  this->reflectance = 0.0f;
}