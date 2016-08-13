#ifndef COLOR_TOOLS_H_
#define COLOR_TOOLS_H_

#include "utility/common.hpp"
#include "utility/linear_alg.hpp"

namespace ray_storm
{
  namespace utility
  {
    class ColorTools
    {

    private:

      ColorTools() {};

    public:

      static float linearToSRGB(float linear)
      {
        return std::min(1.0f, std::max(0.0f, 
          linear <= 0.0031308f ? 12.92f*linear : 1.055f*std::pow(linear, 1.0f/2.4f) - 0.055f));
      }

      static glm::vec3 linearToSRGB(const glm::vec3 &linear)
      {
        return glm::vec3(linearToSRGB(linear.r), linearToSRGB(linear.g), linearToSRGB(linear.b));
      }

      
    };
  }
}

#endif