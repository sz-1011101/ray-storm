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
        const glm::vec3 &lowColor, const glm::vec3 &highColor, float freqency = 16.0f, float curvyness = 0.5f
      ) : lowColor(lowColor), highColor(highColor), freqency(freqency), curvyness(curvyness)
      {

      }

      glm::vec3 sample(const glm::vec2 &uv)
      {
        const glm::vec2 _uv = uv*this->freqency;
        // similar to http://luthuli.cs.uiuc.edu/~daf/courses/ComputerGraphics/Week8/Shading.pdf
        return glm::mix(this->lowColor, this->highColor, 
          std::fmod(this->curvyness + this->curvyness*std::cos(_uv.x)*std::sin(_uv.y) + _uv.x, 1.0f)
        );
      }

    private:

      glm::vec3 lowColor;

      glm::vec3 highColor;

      float freqency;

      float curvyness;
      
    };
  }
}

#endif