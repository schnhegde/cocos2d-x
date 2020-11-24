#include "./WatchVideoPopup.h"
#include "../data/Config.h"
#include "../modules/UiUtil.h"
#include "../PlatformIncludes.h"
#include "../modules/SoundUtil.h"

using data::Config;
using modules::UiUtil;
using modules::SoundUtil;

namespace scenes {
  WatchVideoPopup* WatchVideoPopup::createPopup(std::string reward, int rewardAmount) {
    auto popup =  WatchVideoPopup::create();
    popup->reward = reward;
    popup->rewardAmount = rewardAmount;
    return popup;
  }

  void WatchVideoPopup::swallowTouches() {
    auto swallowTouchListener = cocos2d::EventListenerTouchOneByOne::create();
    swallowTouchListener->setSwallowTouches(true);
    swallowTouchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
      return true;  // if you are consuming it
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouchListener,
      this);
  }

  void WatchVideoPopup::onEnter() {
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

  void WatchVideoPopup::createView() {
    bool is_ad_available = isRewardedAdAvailable();
    Size popupSize = getContentSize();
    messageLayout = CommonLayout::create();
    messageLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    messageLayout->setContentSize(Size(popupSize.width * 0.9, popupSize.height * 0.4));
    char buff[200];
    snprintf(buff, sizeof(buff), "Out of %s? Watch an ad and get +%d %s.", reward.c_str(), rewardAmount, reward.c_str());
    auto messageText = cocos2d::ui::Text::create(is_ad_available ? buff : "Ads not available, try later", Config::FONT_FILE, 26);
    messageText->enableOutline(cocos2d::Color4B::BLACK, 1);
    messageLayout->addChild(messageText);
    messageLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

    buttonsLayout = CommonLayout::create();
    buttonsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    buttonsLayout->setContentSize(Size(popupSize.width * 0.9, popupSize.height * 0.5));
    auto cancelButton = UiUtil::createButton("button_blue_small.png", is_ad_available ? "cancel" : "ok", 26, false);
    cancelButton->addTouchEventListener(
        CC_CALLBACK_2(WatchVideoPopup::CBBtnCancel, this));
    auto watchButton = UiUtil::createButton("button_orange_small.png", "watch ad", 26, false);
    watchButton->addTouchEventListener(
        CC_CALLBACK_2(WatchVideoPopup::CBBtnWatch, this));
    buttonsLayout->addChild(cancelButton);
    if (is_ad_available) {
      buttonsLayout->addChild(watchButton);
    }
    buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
    buttonsLayout->setScale(Config::DSP_SCALE);
    messageLayout->setScale(Config::DSP_SCALE);
    addChild(messageLayout);
    addChild(buttonsLayout);
  }

  void WatchVideoPopup::CBBtnWatch(Ref* sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
      SoundUtil::getInstance()->playEfxBtnTouched();
      if (isRewardedAdAvailable()) {
        showRewardedAd();
        removeFromParent();
      }
    }
  }

  void WatchVideoPopup::CBBtnCancel(Ref* sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
      SoundUtil::getInstance()->playEfxBtnTouched();
      removeFromParent();
    }
  }
}