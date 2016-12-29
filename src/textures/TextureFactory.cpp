#include "textures/TextureFactory.h"
#include "textures/Ring2DTexture.hpp"
#include "textures/Turbulence2DTexture.hpp"

using namespace ray_storm::textures;

Abstract2DTexturePtr<glm::vec3> TextureFactory::createRing2DTexture(const glm::vec3 &c1, const glm::vec3 &c2)
{
  return Abstract2DTexturePtr<glm::vec3>(new Ring2DTexture(c1, c2));
}