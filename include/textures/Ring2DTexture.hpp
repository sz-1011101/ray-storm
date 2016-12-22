#ifndef RING_TEXTURE_H_
#define RING_TEXTURE_H_

namespace ray_storm
{
  namespace textures
  {
    class Ring2DTexture : public Abstract2DTexture<glm::vec3>
    {
    public:

      Ring2DTexture(const glm::vec3 &lowColor, const glm::vec3 &highColor) : lowColor(lowColor), highColor(highColor)
      {

      }

      glm::vec3 sample(const glm::vec2 &uv)
      {
        const glm::vec2 _uv = (uv*2.0f - 1.0f)*4.0f;
        // similar to http://luthuli.cs.uiuc.edu/~daf/courses/ComputerGraphics/Week8/Shading.pdf
        return glm::mix(this->lowColor, this->highColor, 
          std::fmod(0.75f*(1.0f + std::sin(_uv.x*_uv.x))*_uv.x*_uv.x + 
            0.25f*(1.0f + std::cos(_uv.y*_uv.y))*_uv.y*_uv.y, 1.0f)
        );
      }

    private:

      glm::vec3 lowColor;

      glm::vec3 highColor;
      
    };
  }
}

#endif