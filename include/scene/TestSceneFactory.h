#ifndef TEST_SCENE_FACTORY_H_
#define TEST_SCENE_FACTORY_H_

#include "scene/Scene.h"

namespace ray_storm
{
  namespace scene
  {
    class TestSceneFactory
    {

    public:

      static ScenePtr createDefaultScene();

      static ScenePtr createCornellBox();

    private:

      TestSceneFactory() {};
      
    };
  }

}

#endif