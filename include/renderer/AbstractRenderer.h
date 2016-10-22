#ifndef ABSTRACT_RENDERER_H_
#define ABSTRACT_RENDERER_H_

#include <memory>

#include "scene/Scene.h"
#include "camera/AbstractCamera.h"
#include "utility/RenderedData.h"

namespace ray_storm
{
  namespace renderer
  {
    
    class AbstractRenderer
    {

    public:

      virtual ~AbstractRenderer() {};

      virtual void render() = 0;
      
    };
  }
}

#endif