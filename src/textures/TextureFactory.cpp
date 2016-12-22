#include "textures/TextureFactory.h"
#include "textures/Constant2DTexture.hpp"

using namespace ray_storm::textures;

Abstract2DTexturePtr TextureFactory::createConstant2DTexture(const glm::vec3 &constant)
{
  return Abstract2DTexturePtr(new Constant2DTexture(constant));
}