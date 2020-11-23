#include "GameFinishScene.h"

#include "../data/Config.h"
#include "../modules/GameManager.h"
#include "../modules/UiUtil.h"
#include "../modules/SoundUtil.h"
#include "GameScene.h"
#include "LevelSelect.h"
#include "MainMenu.h"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Size;
using cocos2d::SpriteFrameCache;
using cocos2d::ui::Text;
using data::Config;
using modules::GameManager;
using modules::UiUtil;
using modules::SoundUtil;
using std::string;

namespace scenes {

Scene* GameFinishScene::createScene(int movesMade, int levelNo, int starCount) {
  auto instance = GameFinishScene::create();
  instance->starCount = starCount;
  instance->movesMade = movesMade;
  instance->levelNo = levelNo;
  return instance;
}

static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf(
      "Depending on how you compiled you might have to add 'Resources/' in "
      "front of filenames in GameFinishScene.cpp\n");
}

bool GameFinishScene::init() {
  if (!Scene::init()) {
    return false;
  }
  loadPlistFile();
  return true;
}

void GameFinishScene::swallowTouches() {
    auto swallowTouchListener = cocos2d::EventListenerTouchOneByOne::create();
    swallowTouchListener->setSwallowTouches(true);
    swallowTouchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {
      return true;  // if you are consuming it
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(swallowTouchListener,
      this);
  }

void GameFinishScene::addBackButtonListener() {
  auto listener = cocos2d::EventListenerKeyboard::create();
  listener->onKeyReleased = CC_CALLBACK_2(GameFinishScene::onKeyReleased, this);
  cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameFinishScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
  if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK) {
    CBBtnLevelSelect(nullptr, Widget::TouchEventType::ENDED);
  }
}

void GameFinishScene::loadPlistFile() {
  // SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
  // frameCache->addSpriteFramesWithFile("GameFinishScene.plist");
  // frameCache->addSpriteFramesWithFile("Settings.plist");
}

void GameFinishScene::onEnter() {
  Scene::onEnter();
  swallowTouches();
  addBackButtonListener();
  if (starCount > 0) {
    SoundUtil::getInstance()->playEfxGameWon();
  } else {
    SoundUtil::getInstance()->playEfxGameLost();
  }
  Size screenSize = Director::getInstance()->getVisibleSize();
  mainLayout = CommonLayout::create();
  mainLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  mainLayout->setContentSize(screenSize);
  mainLayout->setBackGroundImage("./bg", Widget::TextureResType::PLIST);
  mainLayout->setBackGroundImageScale9Enabled(true);
  addHeaderLayout();
  addDetailLayout();
  addButtonsLayout();
  mainLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  addChild(mainLayout);
}

void GameFinishScene::addHeaderLayout() {
  Size mainSize = mainLayout->getContentSize();
  headerLayout = CommonLayout::create();
  headerLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  headerLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.1));
  CommonLayout* headerBg = CommonLayout::create();
  headerBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  headerBg->setContentSize(Size(mainSize.width * 0.75, mainSize.height * 0.1));
  headerBg->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  headerBg->setBackGroundImageScale9Enabled(true);

  string titleString = starCount <= 0 ? "Level Failed" : "Level Complete";
  Text* title = Text::create(titleString, Config::FONT_FILE, 40);
  title->enableOutline(Color4B::BLACK, 1);

  headerBg->addChild(title);
  headerBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  headerLayout->addChild(headerBg);
  headerLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  headerLayout->setScale(Config::DSP_SCALE);
  mainLayout->addChild(headerLayout);
}

void GameFinishScene::addDetailLayout() {
  using cocos2d::ui::ImageView;

  Size mainSize = mainLayout->getContentSize();
  detailLayout = CommonLayout::create();
  detailLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  detailLayout->setContentSize(
      Size(mainSize.width * 0.75, mainSize.height * 0.4));
  detailLayout->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  detailLayout->setBackGroundImageScale9Enabled(true);

  Size detailSize = detailLayout->getContentSize();

  CommonLayout* starsLayout = CommonLayout::create();
  starsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  starsLayout->setContentSize(Size(detailSize.width, detailSize.height * 0.4));

  for (int i = 1; i <= 3; i++) {
    ImageView* starImage = nullptr;
    if (starCount >= i) {
      starImage = ImageView::create("./star_on", Widget::TextureResType::PLIST);
    } else {
      starImage =
          ImageView::create("./star_off", Widget::TextureResType::PLIST);
    }
    starsLayout->addChild(starImage);
  }

  starsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  CommonLayout* movesLayout = CommonLayout::create();
  movesLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  movesLayout->setContentSize(Size(detailSize.width, detailSize.height * 0.4));
  Text* movesLabel = Text::create("Moves Made", Config::FONT_FILE, 36);
  movesLabel->enableOutline(Color4B::BLACK, 1);
  auto movesDataLayout = CommonLayout::create();
  movesDataLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  movesDataLayout->setContentSize(
      Size(mainSize.height * 0.07, mainSize.height * 0.07));
  movesDataLayout->setBackGroundImage("./dark_bg_tiny",
                                      Widget::TextureResType::PLIST);
  movesDataLayout->setBackGroundImageScale9Enabled(true);
  Text* movesData =
      Text::create(std::to_string(movesMade), Config::FONT_FILE, 32);
  movesData->enableOutline(Color4B::BLACK, 1);
  movesDataLayout->addChild(movesData);
  movesDataLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  movesLayout->addChild(movesLabel);
  movesLayout->addChild(movesDataLayout);
  movesLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  detailLayout->addChild(starsLayout);
  detailLayout->addChild(movesLayout);
  detailLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  detailLayout->setScale(Config::DSP_SCALE);
  mainLayout->addChild(detailLayout);
}

void GameFinishScene::addButtonsLayout() {
  Size mainSize = mainLayout->getContentSize();
  buttonsLayout = CommonLayout::create();
  buttonsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  buttonsLayout->setContentSize(
      Size(mainSize.width * 0.75, mainSize.height * 0.1));

  auto homeButton = UiUtil::createButton("./button_home", "", 10);
  Size homeSize = homeButton->getContentSize();
  CommonLayout* homeBg = CommonLayout::create();
  homeBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  homeBg->setContentSize(Size(homeSize.width * 1.25, homeSize.height * 1.25));
  homeBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
  homeBg->setBackGroundImageScale9Enabled(true);
  homeBg->addChild(homeButton);
  homeBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  homeButton->addTouchEventListener(
      CC_CALLBACK_2(GameFinishScene::CBBtnHome, this));

  auto levelButton = UiUtil::createButton("button_level.png", "", 10, false);
  Size levelSize = levelButton->getContentSize();
  CommonLayout* levelBg = CommonLayout::create();
  levelBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  levelBg->setContentSize(
      Size(levelSize.width * 1.25, levelSize.height * 1.25));
  levelBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
  levelBg->setBackGroundImageScale9Enabled(true);
  levelBg->addChild(levelButton);
  levelBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  levelButton->addTouchEventListener(
      CC_CALLBACK_2(GameFinishScene::CBBtnLevelSelect, this));

  string button_string = starCount > 0 ? "./button_play" : "retry_button_big.png";
  auto playButton = UiUtil::createButton(button_string, "", 10, starCount > 0);
  Size playSize = playButton->getContentSize();
  CommonLayout* playBg = CommonLayout::create();
  playBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  playBg->setContentSize(Size(playSize.width * 1.25, playSize.height * 1.25));
  playBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
  playBg->setBackGroundImageScale9Enabled(true);
  playBg->addChild(playButton);
  playBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  if (starCount > 0) {
    playButton->addTouchEventListener(
      CC_CALLBACK_2(GameFinishScene::CBBtnPlay, this));
  } else {
      playButton->addTouchEventListener(
      CC_CALLBACK_2(GameFinishScene::CBBtnReplay, this));
  }

    homeBg->setScale(Config::DSP_SCALE);
    levelBg->setScale(Config::DSP_SCALE);
    playBg->setScale(Config::DSP_SCALE);
  buttonsLayout->addChild(homeBg);
  buttonsLayout->addChild(levelBg);
  buttonsLayout->addChild(playBg);
  buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  mainLayout->addChild(buttonsLayout);
}

void GameFinishScene::CBBtnHome(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(MainMenu::createScene());
  }
}

void GameFinishScene::CBBtnLevelSelect(Ref* pSender,
                                       Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(LevelSelect::createScene());
  }
}

void GameFinishScene::CBBtnPlay(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(
    GameScene::createScene(GameManager::getInstance()->getCurrentLevel()));
  }
}

void GameFinishScene::CBBtnReplay(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(
    GameScene::createScene(levelNo));
  }
}

}  // namespace scenes
