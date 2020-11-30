#include "./MessagePopup.h"
#include "../data/Config.h"
#include "../modules/UiUtil.h"
#include "../PlatformIncludes.h"
#include "../modules/SoundUtil.h"

using data::Config;
using modules::UiUtil;
using modules::SoundUtil;

namespace scenes {
  MessagePopup* MessagePopup::createPopup(std::string message, MessageType type) {
    auto popup = MessagePopup::create();
    popup->message = message;
    popup->messageType = type;
    return popup;
  }

  void MessagePopup::swallowTouches() {
    auto swallowTouchListener = cocos2d::EventListenerTouchOneByOne::create();
    swallowTouchListener->setSwallowTouches(true);
    swallowTouchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
      return true;  // if you are consuming it
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouchListener,
      this);
  }

  void MessagePopup::onEnter() {
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

  void MessagePopup::createView() {
    Size popupSize = getContentSize();

    string okButtonString = "";
    string cancelButtonString = "";
    int messageSize = 28;

    switch(messageType) {
      case MessageType::SIMPLE_MESSAGE:
        okButtonString = "ok";
        cancelButtonString = "";
        messageSize = 28;
      break;
      case MessageType::REVIEW_MESSAGE:
        okButtonString = "write";
        cancelButtonString = "cancel";
        messageSize = 20;
      break;
    }

    messageLayout = CommonLayout::create();
    messageLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    messageLayout->setContentSize(Size(popupSize.width * 0.9, popupSize.height * 0.4));
    auto messageText = cocos2d::ui::Text::create(message, Config::FONT_FILE, messageSize);
    messageText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    messageText->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    messageText->enableOutline(cocos2d::Color4B::BLACK, 1);
    messageLayout->addChild(messageText);
    messageLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

    buttonsLayout = CommonLayout::create();
    buttonsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    buttonsLayout->setContentSize(Size(popupSize.width * 0.9, popupSize.height * 0.5));

    if (messageType == MessageType::REVIEW_MESSAGE) {
      auto cancelButton = UiUtil::createButton("button_orange_small.png", cancelButtonString, 26, false);
      cancelButton->addTouchEventListener(
              CC_CALLBACK_2(MessagePopup::CBBtnCancel, this));
      buttonsLayout->addChild(cancelButton);
    }

    auto okButton = UiUtil::createButton("button_blue_small.png", okButtonString, 26, false);
    okButton->addTouchEventListener(
        CC_CALLBACK_2(MessagePopup::CBBtnOk, this));
    buttonsLayout->addChild(okButton);



    buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
    buttonsLayout->setScale(Config::DSP_SCALE);
    messageLayout->setScale(Config::DSP_SCALE);
    addChild(messageLayout);
    addChild(buttonsLayout);
  }

  void MessagePopup::CBBtnOk(Ref* sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
      SoundUtil::getInstance()->playEfxBtnTouched();
      switch(messageType) {
        case MessageType::SIMPLE_MESSAGE:
          removeFromParent();
          break;
        case MessageType::REVIEW_MESSAGE:
          openReviewFlow();
        break; 
      }
    }
  }

  void MessagePopup::CBBtnCancel(Ref* sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
      SoundUtil::getInstance()->playEfxBtnTouched();
      removeFromParent();
    }
  }

}