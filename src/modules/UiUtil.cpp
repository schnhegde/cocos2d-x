#include "UiUtil.h"

#include "../data/Config.h"

using cocos2d::Color3B;
using cocos2d::Color4B;
using cocos2d::ui::Widget;
using data::Config;

namespace modules {

int getRandom(int lower, int upper) {
  int num = (rand() % (upper - lower + 1)) + lower;
  return num;
}
Button* UiUtil::createButton(string textureName, string title, int titleSize,
                             bool plist) {
  Button* button = Button::create();
  button->loadTextureNormal(textureName, plist ? Widget::TextureResType::PLIST
                                               : Widget::TextureResType::LOCAL);
  button->setZoomScale(-0.05f);
  button->setTitleText(title);
  button->setTitleFontName(Config::FONT_FILE);
  button->setTitleFontSize(titleSize);
  button->getTitleRenderer()->enableOutline(Color4B::BLACK, 1);
  return button;
}

void UiUtil::transitionFade(cocos2d::Scene* scene) {
  using cocos2d::Director;
  using cocos2d::TransitionSlideInB;
  using cocos2d::TransitionSlideInL;
  using cocos2d::TransitionSlideInR;
  using cocos2d::TransitionSlideInT;
  cocos2d::TransitionScene* transition;

  int randVal = getRandom(0, 3);
  float time = Config::DEFAULT_TRANSITION_TIME;

  switch (randVal) {
    case 0:
      transition = TransitionSlideInR::create(time, scene);
      break;
    case 1:
      transition = TransitionSlideInL::create(time, scene);
      break;
    case 2:
      transition = TransitionSlideInB::create(time, scene);
      break;
    case 3:
      transition = TransitionSlideInT::create(time, scene);
      break;
    default:
      transition = TransitionSlideInT::create(time, scene);
      break;
  }
  Director::getInstance()->replaceScene(transition);
}

void UiUtil::pushFade(cocos2d::Scene* root, cocos2d::Scene* child) {
  using cocos2d::Director;
  using cocos2d::MoveTo;
  using cocos2d::Vec2;

  root->addChild(child, 10);
  child->setName("PauseScene");
  //    child->setScale( 0.0f );
  //    child->setPositionNormalized(Vec2(0.0, 1.0f));
  //    Size rootSize = Director::getInstance()->getVisibleSize();
  //    MoveTo* moveAction = MoveTo::create(0.5f, Vec2(0,0));
  //    child->runAction( moveAction );
}

void UiUtil::removeChild(cocos2d::Node* child) { child->removeFromParent(); }

void UiUtil::popFade(cocos2d::Node* root) {
  using cocos2d::CallFunc;
  using cocos2d::Director;
  using cocos2d::MoveTo;
  using cocos2d::Scene;
  using cocos2d::Sequence;
  using cocos2d::Vec2;

  Scene* pauseScene =
      reinterpret_cast<Scene*>(root->getChildByName("PauseScene"));
  pauseScene->removeFromParent();
}
}  // namespace modules
