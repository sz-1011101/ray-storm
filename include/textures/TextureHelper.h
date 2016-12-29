#ifndef TEXTURE_HELPER_H_
#define TEXTURE_HELPER_H_

#include "utility/common.hpp"

namespace ray_storm
{
  namespace textures
  {
    class TextureHelper
    {
    private:

      TextureHelper() {}

    public:

      static float noise(float p);

      static glm::vec2 noise(const glm::ivec2 &indices);

      /**
       * @brief      perlin noise for unnormalized uv coordinates
       *
       * @param[in]  uv    unormalized uv coordinates
       *
       * @return     perlin value in
       */
      static float perlin(const glm::vec2 &uv);

    };
  }
}

#endif