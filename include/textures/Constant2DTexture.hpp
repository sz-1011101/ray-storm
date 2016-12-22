#ifndef CONSTANT_2D_TEXTURE_H_
#define CONSTANT_2D_TEXTURE_H_

#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace textures
  {
    class Constant2DTexture : public Abstract2DTexture<glm::vec3>
    {
    public:

      Constant2DTexture(const glm::vec3 &constant) : constant(constant) {}

      glm::vec3 sample(const glm::vec2 &uv)
      {
        return this->constant;
      }
    
    private:

      glm::vec3 constant;
      
    };
  }
}

#endif