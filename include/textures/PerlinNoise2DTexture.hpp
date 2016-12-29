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

      PerlinNoise2DTexture(const glm::vec2 &frequency, const T &c1, const T &c2)
        : frequency(frequency), c1(c1), c2(c2) {}

      T sample(const glm::vec2 &uv)
      {
        // http://paulbourke.net/texture_colour/perlin/
        return this->c1 + (this->c2 - this->c1)*glm::clamp((1.0f + TextureHelper::perlin(uv*this->frequency), 0.0f, 1.0f)/2.0f);
      }

    private:

      glm::vec2 frequency;

      T c1;

      T c2;
      
    };
  }
}

#endif