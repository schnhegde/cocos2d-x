#ifndef SRC_SPLASHSCENE_H_
#define SRC_SPLASHSCENE_H_

#include "cocos2d.h"
#include "MainMenu.h"

namespace scenes {
  class SplashScene : public cocos2d::Scene{
    private:
    public:
      CREATE_FUNC(SplashScene);
      static SplashScene* createScene();
      virtual void onEnter();
      void launchMainMenu(float dt);
  };
}

#endif // SRC_SPLASHSCENE_H_