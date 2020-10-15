#include "MainMenu.h"

#include <string>

#include "../data/Config.h"
#include "../modules/GameManager.h"
#include "../modules/SoundUtil.h"
#include "../modules/UiUtil.h"
#include "./LevelSelect.h"
#include "./Settings.h"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Size;
using cocos2d::SpriteFrameCache;
using cocos2d::TextHAlignment;
using cocos2d::TextVAlignment;
using cocos2d::Vec2;
using cocos2d::ui::Text;
using cocos2d::ui::Widget;
using data::Config;
using modules::GameManager;
using modules::SoundUtil;
using modules::UiUtil;
using std::string;

namespace scenes {

Scene* MainMenu::createScene() { return MainMenu::create(); }

static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf(
      "Depending on how you compiled you might have to add 'Resources/' in "
      "front of filenames in MainMenu.cpp\n");
}

bool MainMenu::init() {
  if (!Scene::init()) {
    return false;
  }
  loadPlistFile();
  return true;
}

void MainMenu::loadPlistFile() {
  SpriteFrameCache* frameCache;
  frameCache = SpriteFrameCache::getInstance();
  frameCache->addSpriteFramesWithFile("MainMenu.plist");
  frameCache->addSpriteFramesWithFile("GameFinishScene.plist");
  frameCache->addSpriteFramesWithFile("Settings.plist");
}

void MainMenu::onEnter() {
  Scene::onEnter();
  Size screenSize = Director::getInstance()->getVisibleSize();
  mainLayout = CommonLayout::create();
  mainLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  mainLayout->setContentSize(screenSize);
  mainLayout->setBackGroundImage("./bg", Widget::TextureResType::PLIST);
  mainLayout->setBackGroundImageScale9Enabled(true);
  addHeaderLayout();
  addLogoLayout();
  addButtonsLayout();
  mainLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  addChild(mainLayout);
}

void MainMenu::addHeaderLayout() {
  using cocos2d::ui::Margin;
  Size mainSize = mainLayout->getContentSize();
  headerLayout = CommonLayout::create();
  headerLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  headerLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.1));
  CommonLayout* starBg = CommonLayout::create();
  starBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  starBg->setContentSize(Size(mainSize.width * 0.4, mainSize.height * 0.05));
  starBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
  starBg->setBackGroundImageScale9Enabled(true);

  ImageView* starImage =
      ImageView::create("./star", Widget::TextureResType::PLIST);
  starBg->addChild(starImage);
  int stars_won = GameManager::getInstance()->getTotalStarsWon();
  int total_stars = Config::TOTAL_LEVELS * 3;
  string stars_string = std::to_string(stars_won);
  stars_string += " / " + std::to_string(total_stars);
  Text* stars_text = Text::create(stars_string, Config::FONT_FILE, 26);
  stars_text->setTextHorizontalAlignment(TextHAlignment::CENTER);
  stars_text->setTextVerticalAlignment(TextVAlignment::CENTER);
  stars_text->enableOutline(Color4B::BLACK, 1);
  starBg->addChild(stars_text);
  starBg->justifyChildren(CommonLayout::JUSTIFY::INSIDE);
  Margin textMargin = stars_text->getLayoutParameter()->getMargin();
  textMargin.left = stars_text->getContentSize().width * 0.2;
  stars_text->getLayoutParameter()->setMargin(textMargin);

  headerLayout->addChild(starBg);
  headerLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  mainLayout->addChild(headerLayout);
}

void MainMenu::addLogoLayout() {
  Size mainSize = mainLayout->getContentSize();
  logoLayout = CommonLayout::create();
  logoLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  logoLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.4));

  CommonLayout* logoBg = CommonLayout::create();
  logoBg->setLayoutType(CommonLayout::Type::VERTICAL);
  logoBg->setContentSize(Size(mainSize.width * 0.7, mainSize.height * 0.3));
  logoBg->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  logoBg->setBackGroundImageScale9Enabled(true);

  ImageView* logoImage =
      ImageView::create("./logo", Widget::TextureResType::PLIST);
  logoBg->addChild(logoImage);

  logoBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  logoLayout->addChild(logoBg);

  logoLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  mainLayout->addChild(logoLayout);
}

void MainMenu::addButtonsLayout() {
  Size mainSize = mainLayout->getContentSize();
  buttonsLayout = CommonLayout::create();
  buttonsLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  buttonsLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.4));

  // TODO(sachin): localization
  playButton = UiUtil::createButton("./button_blue", "Play", 36);
  playButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::CBBtnPlay, this));
  Size playSize = playButton->getContentSize();

  CommonLayout* playBg = CommonLayout::create();
  playBg->setLayoutType(CommonLayout::Type::VERTICAL);
  playBg->setContentSize(Size(playSize.width * 1.25, playSize.height * 1.25));
  playBg->setBackGroundImage("./cutout_small", Widget::TextureResType::PLIST);
  playBg->setBackGroundImageScale9Enabled(true);

  settingsButton = UiUtil::createButton("./button_settings", "");
  settingsButton->addTouchEventListener(
      CC_CALLBACK_2(MainMenu::CBBtnSettings, this));
  Size settingsSize = settingsButton->getContentSize();

  CommonLayout* settingsBg = CommonLayout::create();
  settingsBg->setLayoutType(CommonLayout::Type::VERTICAL);
  settingsBg->setContentSize(
      Size(settingsSize.width * 1.25, settingsSize.height * 1.25));
  settingsBg->setBackGroundImage("./cutout_tiny",
                                 Widget::TextureResType::PLIST);
  settingsBg->setBackGroundImageScale9Enabled(true);

  settingsBg->addChild(settingsButton);
  settingsBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  playBg->addChild(playButton);
  playBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  buttonsLayout->addChild(playBg);
  buttonsLayout->addChild(settingsBg);
  buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  mainLayout->addChild(buttonsLayout);
}

void MainMenu::CBBtnPlay(Ref* sender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(LevelSelect::createScene());
  }
}

void MainMenu::CBBtnSettings(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    UiUtil::transitionFade(Settings::createScene(false));
  }
}

}  // namespace scenes
