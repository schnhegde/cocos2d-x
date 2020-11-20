#include "./OutOfMovesPopup.h"
#include "../data/Config.h"
#include "../data/Events.h"
#include "../modules/UiUtil.h"
#include "../PlatformIncludes.h"
#include "../modules/SoundUtil.h"
#include "../modules/EventUtils.h"


using data::Config;
using modules::UiUtil;
using modules::SoundUtil;

namespace scenes {
  OutOfMovesPopup* OutOfMovesPopup::createPopup(int moves) {
    auto popup = OutOfMovesPopup::create();
    popup->moves = moves;
    return popup;
  }

  void OutOfMovesPopup::swallowTouches() {
    auto swallowTouchListener = cocos2d::EventListenerTouchOneByOne::create();
    swallowTouchListener->setSwallowTouches(true);
    swallowTouchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
      return true;  // if you are consuming it
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouchListener,
      this);
  }

  void OutOfMovesPopup::onEnter() {
    CommonLayout::onEnter();
    swallowTouches();
    Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    setLayoutType(CommonLayout::Type::VERTICAL);
    setContentSize(Size(visibleSize.width, visibleSize.height * 0.15));
    setBackGroundImage("./cutout_small", Widget::TextureResType::PLIST);
    setBackGroundImageScale9Enabled(true);
    createView();
    justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  }

  void OutOfMovesPopup::createView() {
    bool is_ad_available = isRewardedAdAvailable();
    if (!is_ad_available) CBBtnCancel(nullptr, Widget::TouchEventType::ENDED);
    Size popupSize = getContentSize();
    messageLayout = CommonLayout::create();
    messageLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    messageLayout->setContentSize(Size(popupSize.width * 0.9, popupSize.height * 0.4));
    char buff[200];
    snprintf(buff, sizeof(buff), "Out of moves? Watch an ad and get +%d moves.", moves);
    auto messageText = cocos2d::ui::Text::create(buff, Config::FONT_FILE, 26);
    messageText->enableOutline(cocos2d::Color4B::BLACK, 1);
    messageLayout->addChild(messageText);
    messageLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

    buttonsLayout = CommonLayout::create();
    buttonsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    buttonsLayout->setContentSize(Size(popupSize.width * 0.9, popupSize.height * 0.5));
    auto cancelButton = UiUtil::createButton("button_blue_small.png", "cancel", 26, false);
    cancelButton->addTouchEventListener(
        CC_CALLBACK_2(OutOfMovesPopup::CBBtnCancel, this));
    auto watchButton = UiUtil::createButton("button_orange_small.png", "watch ad", 26, false);
    watchButton->addTouchEventListener(
        CC_CALLBACK_2(OutOfMovesPopup::CBBtnWatch, this));
    buttonsLayout->addChild(cancelButton);
    buttonsLayout->addChild(watchButton);
    buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
    addChild(messageLayout);
    addChild(buttonsLayout);
  }

  void OutOfMovesPopup::CBBtnWatch(Ref* sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
      SoundUtil::getInstance()->playEfxBtnTouched();
      if (isRewardedAdAvailable()) {
        showRewardedAd();
        removeFromParent();
      }
    }
  }

  void OutOfMovesPopup::CBBtnCancel(Ref* sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
      SoundUtil::getInstance()->playEfxBtnTouched();
      modules::EventUtils::dispatchEvent(data::Events::REWARDED_VIDEO_CANCELLED);
      removeFromParent();
    }
  }
}