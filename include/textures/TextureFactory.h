#ifndef TEXTURE_FACTORY_H_
#define TEXTURE_FACTORY_H_

#include "textures/Abstract2DTexture.h"
#include "textures/Constant2DTexture.hpp"
#include "textures/CheckerBoardTexture.hpp"

namespace ray_storm
{
  namespace textures
  {
    class TextureFactory
    {
    private:

      TextureFactory() {}

    public:

      template<typename T> static Abstract2DTexturePtr<T> createConstant2DTexture(const T &constant)
      {
        return Abstract2DTexturePtr<T>(new Constant2DTexture<T>(constant));
      }

      template<typename T> static Abstract2DTexturePtr<T> createCheckerBoardTexture(const glm::vec2 &freqency, const T &c1, const T &c2)
      {
        return Abstract2DTexturePtr<T>(new CheckerBoardTexture<T>(freqency, c1, c2));
      }
      
    };
  }
}

#endif