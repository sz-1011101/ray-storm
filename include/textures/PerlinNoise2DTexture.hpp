#ifndef PERLIN_NOISE_2D_TEXTURE_H_
#define PERLIN_NOISE_2D_TEXTURE_H_

#include "textures/Abstract2DTexture.h"
#include "textures/TextureHelper.h"

namespace ray_storm
{
  namespace textures
  {
    class PerlinNoise2DTexture : public Abstract2DTexture<glm::vec3>
    {
    public:

      glm::vec3 sample(const glm::vec2 &uv)
      {
        return glm::vec3(TextureHelper::perlin(uv*10.0f));
      }
      
    };
  }
}

#endif