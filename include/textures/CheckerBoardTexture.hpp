#ifndef CHECKER_BOARD_TEXTURE_H_
#define CHECKER_BOARD_TEXTURE_H_

#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace textures
  {

    class CheckerBoardTexture : public Abstract2DTexture<glm::vec3>
    {
    public:

      CheckerBoardTexture(float freqency, const glm::vec3 &color1, const glm::vec3 &color2) :
        freqency(freqency), color1(color1), color2(color2) {}

      glm::vec3 sample(const glm::vec2 &uv)
      {
        const glm::vec2 pos(uv*this->freqency);
        const glm::ivec2 iPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
        if (iPos.x % 2 == iPos.y % 2) // same indices
        {
          return this->color1;
        }
        // different indices
        return this->color2;
      }

    private:

      float freqency;

      glm::vec3 color1;

      glm::vec3 color2;
      
    };

  }
}

#endif