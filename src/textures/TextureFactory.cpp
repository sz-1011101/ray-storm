#include "textures/TextureFactory.h"
#include "textures/Ring2DTexture.hpp"
#include "textures/Turbulence2DTexture.hpp"

using namespace ray_storm::textures;

Abstract2DTexturePtr<glm::vec3> TextureFactory::createRing2DTexture(const glm::vec3 &lowColor, const glm::vec3 &highColor)
{
  return Abstract2DTexturePtr<glm::vec3>(new Ring2DTexture(lowColor, highColor));
}

Abstract2DTexturePtr<glm::vec3> TextureFactory::createTurbulence2DTexture()
{
  return Abstract2DTexturePtr<glm::vec3>(new Turbulence2DTexture);
}