#include "materials/Material.h"

using namespace ray_storm::materials;

Material::Material(
  AbstractBRDFPtr &brdf,
  AbstractBTDFPtr &btdf,
  float indexOfRefraction,
  const glm::vec3 &emittance
) : brdf(brdf), btdf(btdf)
{
  this->indexOfRefraction = indexOfRefraction;
  this->emittance = emittance;
}

Material::Material(
  AbstractBRDFPtr &brdf,
  float indexOfRefraction,
  const glm::vec3 &emittance
  ) : brdf(brdf)
{
  this->emittance = emittance;
  this->indexOfRefraction = indexOfRefraction;
}

Material::Material(
  AbstractBTDFPtr &btdf,
  float indexOfRefraction,
  const glm::vec3 &emittance
) : btdf(btdf)
{
  this->indexOfRefraction = indexOfRefraction;
  this->emittance = emittance;
}