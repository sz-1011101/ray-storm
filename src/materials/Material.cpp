#include "materials/Material.h"

using namespace ray_storm::materials;

Material::Material(AbstractBRDFPtr &brdf, const glm::vec3 &emittance) : brdf(brdf)
{
  this->emittance = emittance;
}

glm::vec3 Material::evaluateBRDF(const glm::vec3 &l, 
  const glm::vec3 &n, const glm::vec3 &v)
{
  if (this->brdf == nullptr)
  {
    return glm::vec3(0.0f);
  }

  return this->brdf->evaluate(l, n, v);
}

void Material::drawReflectedRay(const glm::vec3 &in, const glm::vec3 &position, const glm::vec3 &n, 
  random::RandomizationHelper &randHelper, random::RandomRay &randRay)
{
  // if we dont have a valid brdf, use mirror direction
  if (this->brdf == nullptr)
  {
    const glm::vec3 mirrorDirection = glm::normalize(glm::reflect(-in, n));

    randRay.ray.origin = position;
    randRay.ray.direction = mirrorDirection;
    randRay.inversePDF = 1.0f;
    return;
  }

  this->brdf->drawReflectedRay(in, position, n, randHelper, randRay);
}

glm::vec3 Material::getEmittance() 
{
  return this->emittance;
}

float Material::getFresnelReflectedRatio()
{
  return this->fresnelReflectedRatio;
}

float Material::getFresnelRefractedRatio()
{
  return this->fresnelRefractedRatio;
}