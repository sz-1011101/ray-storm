#ifndef BUMP_MAP_H_
#define BUMP_MAP_H_

#include "geometry/AbstractSurfaceNormalModifier.h"
#include "textures/Abstract2DTexture.h"

namespace ray_storm
{
  namespace geometry
  {
    class BumpMap : public AbstractSurfaceNormalModifier
    {
    public:

      BumpMap(const textures::Abstract2DTexturePtr<float> &bumpMap, float eps) : bumpMap(bumpMap), eps(eps) {}

      glm::vec3 modifyNormal(const glm::vec3 &n, const glm::vec2 &uv)
      {
        // get four height samples
        const float u1 = this->bumpMap->sample(uv + glm::vec2(this->eps, 0.0f));
        const float u2 = this->bumpMap->sample(uv - glm::vec2(this->eps, 0.0f));
        const float v1 = this->bumpMap->sample(uv + glm::vec2(0.0f, this->eps));
        const float v2 = this->bumpMap->sample(uv - glm::vec2(0.0f, this->eps));
        const glm::vec2 gradient(u1 - u2, v1 - v2);

        return utility::Math::localToWorld(n, glm::normalize(glm::vec3(gradient.x, 1.0f, gradient.y))); 
      }

    private:

      textures::Abstract2DTexturePtr<float> bumpMap;
      float eps;
    };
  }
}

#endif