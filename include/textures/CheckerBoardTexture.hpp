#ifndef CHECKER_BOARD_TEXTURE_H_
#define CHECKER_BOARD_TEXTURE_H_

#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace textures
  {

    template<typename T> class CheckerBoardTexture : public Abstract2DTexture<T>
    {
    public:

      CheckerBoardTexture(float freqency, const T &c1, const T &c2) :
        freqency(freqency), c1(c1), c2(c2) {}

      T sample(const glm::vec2 &uv)
      {
        const glm::vec2 pos(uv*this->freqency);
        const glm::ivec2 iPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
        if (iPos.x % 2 == iPos.y % 2) // same indices
        {
          return this->c1;
        }
        // different indices
        return this->c2;
      }

    private:

      float freqency;

      T c1;

      T c2;
      
    };

  }
}

#endif