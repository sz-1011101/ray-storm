#ifndef RING_TEXTURE_H_
#define RING_TEXTURE_H_

#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace textures
  {
    class Ring2DTexture : public Abstract2DTexture<glm::vec3>
    {
    public:

      Ring2DTexture(
        const glm::vec3 &c1, const glm::vec3 &c2, float freqency = 16.0f, float curvyness = 0.5f
      ) : c1(c1), c2(c2), freqency(freqency), curvyness(curvyness)
      {

      }

      glm::vec3 sample(const glm::vec2 &uv)
      {
        const glm::vec2 _uv = uv*this->freqency;
        // similar to http://luthuli.cs.uiuc.edu/~daf/courses/ComputerGraphics/Week8/Shading.pdf
        return glm::mix(this->c1, this->c2, 
          std::fmod(this->curvyness + this->curvyness*std::cos(_uv.x)*std::sin(_uv.y) + _uv.x, 1.0f)
        );
      }

    private:

      glm::vec3 c1;

      glm::vec3 c2;

      float freqency;

      float curvyness;
      
    };
  }
}

#endif