#ifndef ABSTRACT_TEXTURE_H_
#define ABSTRACT_TEXTURE_H_

#include <memory>

#include "utility/common.hpp"

namespace ray_storm
{
  namespace textures
  {
    
    template<typename T> class Abstract2DTexture
    {
    public:

      virtual ~Abstract2DTexture() {};

      /**
       * @brief      Sample the texture via u,v in [0, 1)
       *
       * @param[in]  sampling parameters
       *
       * @return     sampled value at u, v
       */
      virtual T sample(const glm::vec2 &uv) = 0;
      
    };

    template<typename T> using Abstract2DTexturePtr = std::shared_ptr<Abstract2DTexture<T>>;
    
  }
}

#endif