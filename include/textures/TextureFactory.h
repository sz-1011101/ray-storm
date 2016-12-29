#ifndef TEXTURE_FACTORY_H_
#define TEXTURE_FACTORY_H_

#include "textures/Abstract2DTexture.h"
#include "textures/Constant2DTexture.hpp"
#include "textures/CheckerBoardTexture.hpp"
#include "textures/PerlinNoise2DTexture.hpp"
#include "textures/Turbulence2DTexture.hpp"

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

      template<typename T> static Abstract2DTexturePtr<T> createPerlinNoise2DTexture(const glm::vec2 &frequency, const T &c1, const T &c2)
      {
        return Abstract2DTexturePtr<T>(new PerlinNoise2DTexture<T>(frequency, c1, c2));
      }

      template<typename T> static Abstract2DTexturePtr<T> createTurbulence2DTexture(
        int n, float L, float H, float f, const T &c1, const T &c2
      )
      {
        return Abstract2DTexturePtr<T>(new Turbulence2DTexture<T>(n, L, H, f, c1, c2));
      }

      static Abstract2DTexturePtr<glm::vec3> createRing2DTexture(const glm::vec3 &c1, const glm::vec3 &c2);
      
    };
  }
}

#endif