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

      Turbulence2DTexture(int n, float L, float H, float f, const T &c1, const T &c2) : 
        n(n), L(L), H(H), f(f), c1(c1), c2(c2) {}

      T sample(const glm::vec2 &uv)
      {
        float t = 0.0f;

        // add up harmonics
        for (int i = 0; i < this->n; i++)
        {
          t += TextureHelper::perlin(this->f*uv*std::pow(this->L, static_cast<float>(i)))/std::pow(this->L, i*this->H);
        }

        return this->c1 + (this->c2 - this->c1)*((1.0f + t)/2.0f);
      }

    private:

      int n;
      float L, H, f;

      T c1;

      T c2;

      
    };
  }
}

#endif