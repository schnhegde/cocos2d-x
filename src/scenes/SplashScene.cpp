#include "SplashScene.h"
#include <ui/CocosGUI.h>

#include "../modules/CommonLayout.h"

using modules::CommonLayout;

namespace scenes {

  SplashScene* SplashScene::createScene() {
    return SplashScene::create();
  }

  void SplashScene::onEnter() {
    cocos2d::Scene::onEnter();
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    CommonLayout* mainLayout = CommonLayout::create();
    mainLayout->setLayoutType(CommonLayout::Type::VERTICAL);
    mainLayout->setContentSize(visibleSize);
    mainLayout->setBgColor(cocos2d::Color3B(230,230,230));
    cocos2d::ui::ImageView* splashImage = cocos2d::ui::ImageView::create("splash.png");
    splashImage->setPositionNormalized(cocos2d::Vec2(0.5, 0.5));
    splashImage->setContentSize(visibleSize);
    mainLayout->addChild(splashImage);
    mainLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
    addChild(mainLayout);
    this->scheduleOnce(schedule_selector(SplashScene::launchMainMenu), 0.8f);
  }

  void SplashScene::launchMainMenu(float dt) {
    cocos2d::Director::getInstance()->replaceScene(MainMenu::createScene());
  }

}