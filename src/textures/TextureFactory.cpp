#include "textures/TextureFactory.h"
#include "textures/Constant2DTexture.hpp"
#include "textures/CheckerBoardTexture.hpp"

using namespace ray_storm::textures;

Abstract2DTexturePtr TextureFactory::createConstant2DTexture(const glm::vec3 &constant)
{
  return Abstract2DTexturePtr(new Constant2DTexture(constant));
}

Abstract2DTexturePtr TextureFactory::createCheckerBoardTexture(float freqency, const glm::vec3 &color1, const glm::vec3 &color2)
{
  return Abstract2DTexturePtr(new CheckerBoardTexture(freqency, color1, color2));
}