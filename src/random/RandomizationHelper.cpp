#include "random/RandomizationHelper.h"

using namespace ray_storm::random;

glm::vec3 RandomizationHelper::drawUniformRandomSphereDirection(MTEngine &engine) 
{
  // sphere point picking, see http://mathworld.wolfram.com/SpherePointPicking.html
  const float u = engine.draw();
  const float v = engine.draw();

  const float theta = 2.0f*M_PI*u;
  const float phi = std::acos(2.0f*v - 1.0f);

  const float cosTheta = std::cos(theta);
  const float cosPhi = std::cos(phi);
  const float sinTheta = std::sin(theta);
  const float sinPhi = std::sin(phi);

  return glm::vec3(cosTheta*sinPhi, sinTheta*sinPhi, cosPhi);
}

glm::vec3 RandomizationHelper::drawUniformRandomHemisphereDirection(MTEngine &engine, const glm::vec3 &n)
{
  glm::vec3 sphereDir = RandomizationHelper::drawUniformRandomSphereDirection(engine);

  // flip around if below hemisphere
  if (glm::dot(n, sphereDir) < 0.0f) {
    return -sphereDir;
  }

  return sphereDir;
}

glm::vec3 RandomizationHelper::drawCosineWeightedRandomHemisphereDirection(MTEngine &engine, const glm::vec3 &n, float e)
{
  const float u = engine.draw();
  const float v = engine.draw();

  glm::vec3 dir;

  const float sqrtTerm = std::sqrt(1.0f - std::pow(v, 2.0f/(e + 1.0f)));

  dir.x = sqrtTerm*std::cos(2.0f*M_PI*u);
  dir.z = sqrtTerm*std::sin(2.0f*M_PI*u);
  dir.y = std::pow(v, 1.0f/(e + 1.0f));

  return RandomizationHelper::transform(n, glm::normalize(dir));
}

glm::vec3 RandomizationHelper::transform(const glm::vec3 &n, const glm::vec3 &vector)
{
  glm::vec3 u = glm::cross(n, glm::vec3(1, 0, 0));
  // HACK but it seems to work decently
  if (std::abs(u.x) < 0.0001f && std::abs(u.y) < 0.0001f && std::abs(u.z) < 0.0001f)
  {
    u = glm::cross(n, glm::vec3(0, 1, 0));
  }

  glm::vec3 v = glm::cross(n, u);

  u = glm::normalize(u);
  v = glm::normalize(v);

  const glm::mat3 transformMat(v.x, v.y, v.z,
    n.x, n.y, n.z,
    u.x, u.y, u.z
    );

  const glm::vec3 trans = glm::normalize(transformMat*vector);

  return trans;
}

float RandomizationHelper::cosineRandomHemisphereInversePDF(float cosTheta, float e)
{
  return 1.0f/std::pow(cosTheta, e)*(e + 1.0f)/(2.0f*M_PI);
}