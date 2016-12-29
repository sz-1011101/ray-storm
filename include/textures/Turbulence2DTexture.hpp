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

      Turbulence2DTexture(int n, float a, float b, float f, const T &c1, const T &c2) : 
        n(n), a(a), b(b), f(f), c1(c1), c2(c2) {}

      T sample(const glm::vec2 &uv)
      {
        float t = 0.0f;
        
        // add up harmonics
        for (int i = 0; i < this->n; i++)
        {
          const float pow1 = std::pow(this->b, static_cast<float>(i));
          const float pow2 = std::pow(this->a, static_cast<float>(i));
          t += TextureHelper::perlin(this->f*uv*pow1)/pow2;
        }

        return this->c1 + (this->c2 - this->c1)*((1.0f + t)/2.0f);
      }

    private:

      int n;
      float a, b, f;

      T c1;

      T c2;
      
    };
  }
}

#endif