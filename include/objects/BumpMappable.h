#ifndef BUMP_MAPPABLE_H_
#define BUMP_MAPPABLE_H_

#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace objects
  {
    class BumpMappable
    {
    public:

      virtual ~BumpMappable() {};

      void setBumpMap(const textures::Abstract2DTexturePtr<float> &bumpMap);

    protected:

      virtual glm::vec3 computeNormal(const glm::vec3 &n, const glm::vec2 &uv);

    private:

      textures::Abstract2DTexturePtr<float> bumpMap;

    };
  }
}


#endif