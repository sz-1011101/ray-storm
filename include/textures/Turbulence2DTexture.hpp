#ifndef TURBULENCE_TEXTURE_H_
#define TURBULENCE_TEXTURE_H_

#include "textures/Abstract2DTexture.h"
#include "textures/TextureHelper.h"

namespace ray_storm
{
  namespace textures
  {
    class Turbulence2DTexture : public Abstract2DTexture<glm::vec3>
    {
    public:

      glm::vec3 sample(const glm::vec2 &uv)
      {
        const int n = 5;
        const float L = 2.0f;
        const float H = 1.0f;

        float t(0.0f);

        for (int i = 0; i < n; i++)
        {
          t += std::pow(TextureHelper::perlin(5.0f*uv*std::pow(L, static_cast<float>(i))), 5.0f)/std::pow(L, i*H);
        }

        return glm::vec3(glm::clamp((1.0f + std::cos(t))/2.0f, 0.0f, 1.0f));
      }

      
    };
  }
}

#endif