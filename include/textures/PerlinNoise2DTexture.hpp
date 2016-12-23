#ifndef PERLIN_NOISE_2D_TEXTURE_H_
#define PERLIN_NOISE_2D_TEXTURE_H_

#include "textures/Abstract2DTexture.h"
#include "textures/TextureHelper.h"

namespace ray_storm
{
  namespace textures
  {
    template<typename T> class PerlinNoise2DTexture : public Abstract2DTexture<T>
    {
    public:

      PerlinNoise2DTexture(const glm::vec2 &frequency, const T &min, const T &max)
        : frequency(frequency), min(min), max(max) {}

      T sample(const glm::vec2 &uv)
      {
        return min + (max - min)*TextureHelper::perlin(uv*frequency);
      }

    private:

      glm::vec2 frequency;

      T min;

      T max;
      
    };
  }
}

#endif