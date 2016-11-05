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

      static ScenePtr createCornellBox(bool naturalLighting, bool lightSources);

      static ScenePtr createReflectionTest();

    private:

      TestSceneFactory() {};

      static ScenePtr buildBox();

      static ScenePtr buildBigWall();
      
    };
  }

}

#endif