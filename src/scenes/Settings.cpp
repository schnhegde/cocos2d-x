#include "Settings.h"

#include <string>

#include "../data/Config.h"
#include "../modules/GameManager.h"
#include "../modules/UiUtil.h"
#include "../modules/SoundUtil.h"
#include "./MainMenu.h"
#include "../PlatformIncludes.h"

using modules::SoundUtil;
using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Size;
using cocos2d::SpriteFrameCache;
using cocos2d::TextHAlignment;
using cocos2d::TextVAlignment;
using cocos2d::Vec2;
using cocos2d::ui::Margin;
using cocos2d::ui::Text;
using cocos2d::ui::Widget;
using data::Config;
using modules::GameManager;
using modules::UiUtil;
using scenes::MainMenu;
using std::string;

namespace scenes {

Scene* Settings::createScene(bool isPaused) {
  auto instance = Settings::create();
  instance->isPaused = isPaused;
  return instance;
}

static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf(
      "Depending on how you compiled you might have to add 'Resources/' in "
      "front of filenames in Settings.cpp\n");
}

bool Settings::init() {
  if (!Scene::init()) {
    return false;
  }
  loadPlistFile();
  return true;
}

void Settings::loadPlistFile() {
  // SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
  // frameCache->addSpriteFramesWithFile("Settings.plist");
}

void Settings::onEnter() {
  Scene::onEnter();
  Size screenSize = Director::getInstance()->getVisibleSize();
  mainLayout = CommonLayout::create();
  mainLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  mainLayout->setContentSize(screenSize);
  mainLayout->setBackGroundImage("./bg", Widget::TextureResType::PLIST);
  mainLayout->setBackGroundImageScale9Enabled(true);
  addHeaderLayout();
  addButtonsLayout();
  addMessageLayout();
  mainLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  addChild(mainLayout);
}

void Settings::addHeaderLayout() {
  Size mainSize = mainLayout->getContentSize();
  headerLayout = CommonLayout::create();
  headerLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  headerLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.1));
  CommonLayout* headerBg = CommonLayout::create();
  headerBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  headerBg->setContentSize(Size(mainSize.width * 0.75, mainSize.height * 0.1));
  headerBg->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  headerBg->setBackGroundImageScale9Enabled(true);

  backButton = UiUtil::createButton("button_back.png", "", 10, false);
  backButton->addTouchEventListener(CC_CALLBACK_2(Settings::CBBtnBack, this));
  backButton->setZoomScale(-0.05f);

  string titleString = isPaused ? "Paused" : "Settings";
  Text* title = Text::create(titleString, Config::FONT_FILE, 40);
  title->enableOutline(Color4B::BLACK, 1);

  headerBg->addChild(backButton);
  headerBg->addChild(title);
  headerBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  headerLayout->addChild(headerBg);
  headerLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  Margin backMargin = backButton->getLayoutParameter()->getMargin();
  backMargin.left = mainSize.width * 0.025;
  backButton->getLayoutParameter()->setMargin(backMargin);
  mainLayout->addChild(headerLayout);
}

void Settings::addButtonsLayout() {
  Size mainSize = mainLayout->getContentSize();
  buttonsLayout = CommonLayout::create();
  buttonsLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  buttonsLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.5));

  if (isPaused) {
    CommonLayout* navButtonLayout = CommonLayout::create();
    navButtonLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    navButtonLayout->setContentSize(
        Size(mainSize.width * 0.9, mainSize.height * 0.1));

    homeButton = UiUtil::createButton("./button_home", "", 10);
    homeButton->addTouchEventListener(CC_CALLBACK_2(Settings::CBBtnHome, this));
    Size homeSize = homeButton->getContentSize();
    CommonLayout* homeBg = CommonLayout::create();
    homeBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
    homeBg->setContentSize(Size(homeSize.width * 1.25, homeSize.height * 1.25));
    homeBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
    homeBg->setBackGroundImageScale9Enabled(true);
    homeBg->addChild(homeButton);
    homeBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

    playButton = UiUtil::createButton("./button_play", "");
    playButton->addTouchEventListener(CC_CALLBACK_2(Settings::CBBtnPlay, this));
    Size playSize = homeButton->getContentSize();
    CommonLayout* playBg = CommonLayout::create();
    playBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
    playBg->setContentSize(Size(playSize.width * 1.25, playSize.height * 1.25));
    playBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
    playBg->setBackGroundImageScale9Enabled(true);
    playBg->addChild(playButton);
    playBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

    navButtonLayout->addChild(homeBg);
    navButtonLayout->addChild(playBg);
    navButtonLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
    buttonsLayout->addChild(navButtonLayout);
  }

  CommonLayout* toggleLayout = CommonLayout::create();
  toggleLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  toggleLayout->setContentSize(
      Size(mainSize.width * 0.9, mainSize.height * 0.1));

  bool sound_state = GameManager::getInstance()->getSoundState();
  string texture_name = "./button_sound_";
  texture_name += (sound_state ? "on" : "off");
  soundButton = UiUtil::createButton(texture_name, "", 10);
  soundButton->addTouchEventListener(CC_CALLBACK_2(Settings::CBBtnSound, this));
  Size soundSize = soundButton->getContentSize();
  CommonLayout* soundBg = CommonLayout::create();
  soundBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  soundBg->setContentSize(
      Size(soundSize.width * 1.25, soundSize.height * 1.25));
  soundBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
  soundBg->setBackGroundImageScale9Enabled(true);
  soundBg->addChild(soundButton);
  soundBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  bool music_state = GameManager::getInstance()->getMusicState();
  texture_name = "./button_music_";
  texture_name += (music_state ? "on" : "off");
  musicButton = UiUtil::createButton(texture_name, "");
  musicButton->addTouchEventListener(CC_CALLBACK_2(Settings::CBBtnMusic, this));
  Size musicSize = musicButton->getContentSize();
  CommonLayout* musicBg = CommonLayout::create();
  musicBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  musicBg->setContentSize(
      Size(musicSize.width * 1.25, musicSize.height * 1.25));
  musicBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
  musicBg->setBackGroundImageScale9Enabled(true);
  musicBg->addChild(musicButton);
  musicBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  toggleLayout->addChild(soundBg);
  toggleLayout->addChild(musicBg);
  toggleLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  buttonsLayout->addChild(toggleLayout);

  // TODO(sachin): uncomment this once iap is done.
  if (GameManager::getInstance()->getBillingConnected() && !GameManager::getInstance()->isGameOwned()) {
    buyButton = UiUtil::createButton("button_orange_big.png", "Buy", 34, false);
    buyButton->addTouchEventListener(CC_CALLBACK_2(Settings::CBBtnBuy, this));
    Size buySize = buyButton->getContentSize();
    CommonLayout* buyBg = CommonLayout::create();
    buyBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
    buyBg->setContentSize(Size(buySize.width * 1.25, buySize.height * 1.25));
    buyBg->setBackGroundImage("./cutout_small", Widget::TextureResType::PLIST);
    buyBg->setBackGroundImageScale9Enabled(true);
    buyBg->addChild(buyButton);
    buyBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

    buttonsLayout->addChild(buyBg);
  }

  feedbackButton = UiUtil::createButton("button_orange_big.png", "Feedback", 34, false);
  feedbackButton->addTouchEventListener(CC_CALLBACK_2(Settings::CBBtnFeedback, this));
  Size feedBackSize = feedbackButton->getContentSize();
  CommonLayout* feedBackBg = CommonLayout::create();
  feedBackBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  feedBackBg->setContentSize(Size(feedBackSize.width * 1.25, feedBackSize.height * 1.25));
  feedBackBg->setBackGroundImage("./cutout_small", Widget::TextureResType::PLIST);
  feedBackBg->setBackGroundImageScale9Enabled(true);
  feedBackBg->addChild(feedbackButton);
  feedBackBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  buttonsLayout->addChild(feedBackBg);

  buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  mainLayout->addChild(buttonsLayout);
}

void Settings::addMessageLayout() {
  Size mainSize = mainLayout->getContentSize();
  messageLayout = CommonLayout::create();
  messageLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  messageLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.15));

  CommonLayout* messageBg = CommonLayout::create();
  messageBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  messageBg->setContentSize(Size(mainSize.width * 0.8, mainSize.height * 0.15));
  messageBg->setBackGroundImage("./cutout_small",
                                Widget::TextureResType::PLIST);
  messageBg->setBackGroundImageScale9Enabled(true);
  // TODO(sachin): localization
  std::string message1 =
      "Thanks for playing our game!\n\n"
      "Please feel free to click on feedback button above\nand let us know if you faced any issue."
      "\n\nSelect your email client after clicking on feedback.";
  std::string message2 =
      "Thanks for buying a copy of our game!\n\n"
      "Please feel free to click on feedback button above\nand let us know if you faced any issue."
      "\n\nSelect your email client after clicking on feedback.";
  std::string message = GameManager::getInstance()->isGameOwned() ? message2 : message1;
  Text* messageText = Text::create(message, Config::FONT_FILE, 20);
  messageText->enableOutline(Color4B::BLACK, 1);
  messageText->setTextVerticalAlignment(TextVAlignment::CENTER);
  messageText->setTextHorizontalAlignment(TextHAlignment::CENTER);
  messageBg->addChild(messageText);
  messageBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  messageLayout->addChild(messageBg);

  messageLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  mainLayout->addChild(messageLayout);
}

void Settings::CBBtnBack(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    if (!isPaused) {
      UiUtil::transitionFade(MainMenu::createScene());
    } else {
      UiUtil::popFade(getParent());
    }
  }
}

void Settings::CBBtnHome(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(MainMenu::createScene());
  }
}

void Settings::CBBtnPlay(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::popFade(getParent());
  }
}

void Settings::CBBtnSound(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    bool sound_state = GameManager::getInstance()->getSoundState();
    sound_state = !sound_state;
    GameManager::getInstance()->setSoundState(sound_state);
    string texture_name = "./button_sound_";
    texture_name += (sound_state ? "on" : "off");
    soundButton->loadTextureNormal(texture_name, Widget::TextureResType::PLIST);
    SoundUtil::getInstance()->updateIsSoundOff();
  }
}

void Settings::CBBtnMusic(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    bool music_state = GameManager::getInstance()->getMusicState();
    music_state = !music_state;
    GameManager::getInstance()->setMusicState(music_state);
    string texture_name = "./button_music_";
    texture_name += (music_state ? "on" : "off");
    musicButton->loadTextureNormal(texture_name, Widget::TextureResType::PLIST);
  }
}

void Settings::CBBtnBuy(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    purchase("buy_game");
  }
}

void Settings::CBBtnFeedback(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    openFeedbackEmail();
  }
}

}  // namespace scenes
