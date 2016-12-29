#ifndef TURBULENCE_TEXTURE_H_
#define TURBULENCE_TEXTURE_H_

#include "textures/Abstract2DTexture.h"
#include "textures/TextureHelper.h"

namespace ray_storm
{
  namespace textures
  {
    template<typename T> class Turbulence2DTexture : public Abstract2DTexture<T>
    {
    public:

      Turbulence2DTexture(int n, float L, float H, float f, const T &min, const T &max) : 
        n(n), L(L), H(H), f(f), min(min), max(max) {}

      T sample(const glm::vec2 &uv)
      {
        float t = 0.0f;

        // add up harmonics
        for (int i = 0; i < this->n; i++)
        {
          t += TextureHelper::perlin(this->f*uv*std::pow(this->L, static_cast<float>(i)))/std::pow(this->L, i*this->H);
        }

        return this->min + (this->max - this->min)*((1.0f + t)/2.0f);
      }

    private:

      int n;
      float L, H, f;

      T min;

      T max;

      
    };
  }
}

#endif