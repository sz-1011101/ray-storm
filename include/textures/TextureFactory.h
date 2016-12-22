#ifndef TEXTURE_FACTORY_H_
#define TEXTURE_FACTORY_H_

#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace textures
  {
    class TextureFactory
    {
    private:

      TextureFactory() {}

    public:

      static Abstract2DTexturePtr createConstant2DTexture(const glm::vec3 &constant);

      static Abstract2DTexturePtr createCheckerBoardTexture(float freqency, const glm::vec3 &color1, const glm::vec3 &color2);
      
    };
  }
}

#endif