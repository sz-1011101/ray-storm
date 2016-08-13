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

glm::vec3 RandomizationHelper::drawUniformRandomHemisphereDirection(MTEngine &engine, const glm::vec3 &normal)
{
  glm::vec3 sphereDir = RandomizationHelper::drawUniformRandomSphereDirection(engine);

  // flip around if below hemisphere
  if (glm::dot(normal, sphereDir) < 0.0f) {
    return -sphereDir;
  }

  return sphereDir;
}