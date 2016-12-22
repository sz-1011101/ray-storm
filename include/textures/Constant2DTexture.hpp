#ifndef CONSTANT_2D_TEXTURE_H_
#define CONSTANT_2D_TEXTURE_H_

#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace textures
  {
    template<typename T> class Constant2DTexture : public Abstract2DTexture<T>
    {
    public:

      Constant2DTexture(const T &constant) : constant(constant) {}

      T sample(const glm::vec2 &uv)
      {
        return this->constant;
      }
    
    private:

      T constant;
      
    };
  }
}

#endif