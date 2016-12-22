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
      
    };
  }
}

#endif