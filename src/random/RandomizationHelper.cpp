#include "random/RandomizationHelper.h"

using namespace ray_storm::random;

glm::vec3 RandomizationHelper::drawUniformRandomSphereDirection() 
{
  // sphere point picking, see http://mathworld.wolfram.com/SpherePointPicking.html
  const float u = this->engine.draw();
  const float v = this->engine.draw();

  const float theta = 2.0f*M_PI*u;
  const float phi = std::acos(2.0f*v - 1.0f);

  const float cosTheta = std::cos(theta);
  const float cosPhi = std::cos(phi);
  const float sinTheta = std::sin(theta);
  const float sinPhi = std::sin(phi);

  return glm::vec3(cosTheta*sinPhi, sinTheta*sinPhi, cosPhi);
}

glm::vec3 RandomizationHelper::drawUniformRandomHemisphereDirection(const glm::vec3 &n)
{
  glm::vec3 sphereDir = this->drawUniformRandomSphereDirection();

  // flip around if below hemisphere
  if (glm::dot(n, sphereDir) < 0.0f) {
    return -sphereDir;
  }

  return sphereDir;
}

glm::vec3 RandomizationHelper::drawCosineWeightedRandomHemisphereDirection(const glm::vec3 &n, float e)
{
  const float u = this->engine.draw();
  const float v = this->engine.draw();

  glm::vec3 dir;

  const float sqrtTerm = std::sqrt(1.0f - std::pow(v, 2.0f/(e + 1.0f)));

  dir.x = sqrtTerm*std::cos(2.0f*M_PI*u);
  dir.z = sqrtTerm*std::sin(2.0f*M_PI*u);
  dir.y = std::pow(v, 1.0f/(e + 1.0f));

  return utility::Math::localToWorld(n, glm::normalize(dir));
}

glm::vec3 RandomizationHelper::drawCosineDistributedDirection(const glm::vec3 &n)
{
  const float u = this->engine.draw();
  const float v = this->engine.draw();
  const float theta = 2.0f*M_PI*v;
  const float phi = std::acos(std::sqrt(1.0f - u));
  return utility::Math::localToWorld(n, utility::Math::sphericalToCartesian(glm::vec3(1.0f, theta, phi)));
}

float RandomizationHelper::cosineRandomHemispherePDF(float cosTheta, float e)
{
  return cosTheta > 0.0f ? std::pow(cosTheta, e)*(e + 1.0f)/(2.0f*M_PI) : 0.0f;
}