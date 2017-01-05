#include "objects/BumpMappable.h"
#include "utility/common.hpp"

using namespace ray_storm::objects;

void BumpMappable::setBumpMap(const textures::Abstract2DTexturePtr<float> &bumpMap)
{
  this->bumpMap = bumpMap;
}

glm::vec3 BumpMappable::computeNormal(const glm::vec3 &n, const glm::vec2 &uv)
{
  if (this->bumpMap == nullptr)
  {
    return n;
  }
  const float eps = 0.01f;
  // get four height samples
  const float u1 = this->bumpMap->sample(uv + glm::vec2(eps, 0.0f));
  const float u2 = this->bumpMap->sample(uv - glm::vec2(eps, 0.0f));
  const float v1 = this->bumpMap->sample(uv + glm::vec2(0.0f, eps));
  const float v2 = this->bumpMap->sample(uv - glm::vec2(0.0f, eps));
  const glm::vec2 gradient(u1 - u2, v1 - v2);

  return utility::Math::localToWorld(n, glm::normalize(glm::vec3(gradient.x, 1.0f, gradient.y))); 
}