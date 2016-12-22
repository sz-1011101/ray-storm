#ifndef ABSTRACT_TEXTURE_H_
#define ABSTRACT_TEXTURE_H_

#include <memory>

#include "utility/common.hpp"

namespace ray_storm
{
  namespace textures
  {
    
    class Abstract2DTexture
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
      virtual glm::vec3 sample(const glm::vec2 &uv) = 0;
      
    };

    typedef std::shared_ptr<Abstract2DTexture> Abstract2DTexturePtr;
    
  }
}

#endif