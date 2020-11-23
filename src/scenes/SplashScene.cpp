#include "SplashScene.h"
#include <ui/CocosGUI.h>

namespace scenes {

  SplashScene* SplashScene::createScene() {
    return SplashScene::create();
  }

  void SplashScene::onEnter() {
    cocos2d::Scene::onEnter();
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::ui::Scale9Sprite* splashImage = cocos2d::ui::Scale9Sprite::create("splash.png");
    splashImage->setPositionNormalized(cocos2d::Vec2(0.5, 0.5));
    splashImage->setContentSize(visibleSize);
    splashImage->setScale9Enabled(true);
    addChild(splashImage);
    this->scheduleOnce(schedule_selector(SplashScene::launchMainMenu), 0.8f);
  }

  void SplashScene::launchMainMenu(float dt) {
    cocos2d::Director::getInstance()->replaceScene(MainMenu::createScene());
  }

}