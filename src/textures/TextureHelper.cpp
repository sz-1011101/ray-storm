#include "textures/TextureHelper.h"

using namespace ray_storm::textures;

// rand and noise adapted from https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
// totally not predictable random numbers...

float TextureHelper::noise(float p)
{
  return glm::fract(glm::sin(p) * 43758.5453);
}

glm::vec2 TextureHelper::noise(const glm::ivec2 &indices)
{
  return glm::fract(glm::vec2(
    glm::sin(glm::dot(glm::vec2(indices), glm::vec2(12.9898,78.233))) * 43758.5453,
    glm::cos(glm::dot(glm::vec2(indices), glm::vec2(65.0212,12.723))) * 43758.5453
    )
  );
}

float TextureHelper::perlin(const glm::vec2 &uv)
{
  const glm::ivec2 topLeftIndices(static_cast<int>(std::floor(uv.x)), static_cast<int>(std::floor(uv.y)));
  const glm::ivec2 topRightIndices(topLeftIndices + glm::ivec2(1, 0));
  const glm::ivec2 bottomLeftIndices(topLeftIndices + glm::ivec2(0, 1));
  const glm::ivec2 bottomRightIndices(topLeftIndices + glm::ivec2(1, 1));
  // gradiants are likely slightly biased 
  const glm::vec2 gradiantTL = glm::normalize(2.0f*TextureHelper::noise(topLeftIndices) - 1.0f);
  const glm::vec2 gradiantTR = glm::normalize(2.0f*TextureHelper::noise(topRightIndices) - 1.0f);
  const glm::vec2 gradiantBL = glm::normalize(2.0f*TextureHelper::noise(bottomLeftIndices) - 1.0f);
  const glm::vec2 gradiantBR = glm::normalize(2.0f*TextureHelper::noise(bottomRightIndices) - 1.0f);

  const glm::vec2 diffTL = uv - glm::vec2(topLeftIndices);
  const glm::vec2 diffTR = uv - glm::vec2(topRightIndices);
  const glm::vec2 diffBL = uv - glm::vec2(bottomLeftIndices);
  const glm::vec2 diffBR = uv - glm::vec2(bottomRightIndices);

  const float tl = glm::clamp(glm::dot(diffTL, gradiantTL), -1.0f, 1.0f);
  const float tr = glm::clamp(glm::dot(diffTR, gradiantTR), -1.0f, 1.0f);
  const float bl = glm::clamp(glm::dot(diffBL, gradiantBL), -1.0f, 1.0f);
  const float br = glm::clamp(glm::dot(diffBR, gradiantBR), -1.0f, 1.0f);

  float top = tl + (tr - tl)*glm::smoothstep(0.0f, 1.0f, diffTL.x);
  float bot = bl + (br - bl)*glm::smoothstep(0.0f, 1.0f, diffBL.x);
  return (1.0f + glm::clamp(top + (bot - top)*glm::smoothstep(0.0f, 1.0f, diffTL.y), -1.0f, 1.0f))/2.0f;
}