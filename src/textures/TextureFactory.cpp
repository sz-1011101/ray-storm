#include "textures/TextureFactory.h"
#include "textures/Constant2DTexture.hpp"
#include "textures/CheckerBoardTexture.hpp"

using namespace ray_storm::textures;

Abstract2DTexturePtr<glm::vec3> TextureFactory::createConstant2DTexture(const glm::vec3 &constant)
{
  return Abstract2DTexturePtr<glm::vec3>(new Constant2DTexture(constant));
}

Abstract2DTexturePtr<glm::vec3> TextureFactory::createCheckerBoardTexture(float freqency, const glm::vec3 &color1, const glm::vec3 &color2)
{
  return Abstract2DTexturePtr<glm::vec3>(new CheckerBoardTexture(freqency, color1, color2));
}