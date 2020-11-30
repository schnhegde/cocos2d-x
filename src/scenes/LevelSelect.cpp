#include "LevelSelect.h"

#include <string>

#include "../data/Config.h"
#include "../modules/GameManager.h"
#include "../modules/UiUtil.h"
#include "../modules/SoundUtil.h"
#include "./GameScene.h"
#include "./MainMenu.h"

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

Scene* LevelSelect::createScene() {
  auto instance = LevelSelect::create();
  return instance;
}

static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf(
      "Depending on how you compiled you might have to add 'Resources/' in "
      "front of filenames in LevelSelect.cpp\n");
}

bool LevelSelect::init() {
  if (!Scene::init()) {
    return false;
  }
  return true;
}

void LevelSelect::addBackButtonListener() {
  auto listener = cocos2d::EventListenerKeyboard::create();
  listener->onKeyReleased = CC_CALLBACK_2(LevelSelect::onKeyReleased, this);
  cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void LevelSelect::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
  if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK) {
    CBBtnBack(nullptr, Widget::TouchEventType::ENDED);
  }
}

void LevelSelect::onEnter() {
  Scene::onEnter();
  addBackButtonListener();
  Size screenSize = Director::getInstance()->getVisibleSize();
  mainLayout = CommonLayout::create();
  mainLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  mainLayout->setContentSize(screenSize);
  mainLayout->setBackGroundImage("./bg", Widget::TextureResType::PLIST);
  mainLayout->setBackGroundImageScale9Enabled(true);
  addHeaderLayout();
  addGridLayout();
  addButtonsLayout();
  mainLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  addChild(mainLayout);
}

void LevelSelect::addHeaderLayout() {
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
  backButton->addTouchEventListener(
      CC_CALLBACK_2(LevelSelect::CBBtnBack, this));

  string titleString = "Select Level";
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
  headerLayout->setScale(Config::DSP_SCALE);
  mainLayout->addChild(headerLayout);
}

void LevelSelect::addGridLayout() {
  Size mainSize = mainLayout->getContentSize();
  gridLayout = CommonLayout::create();
  gridLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  gridLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.5));
  int currentLevel = GameManager::getInstance()->getCurrentLevel();
  startValue = ((std::ceil(currentLevel / 16)) * 16) + 1;

  for (int i = 0; i < 4; i++) {
    CommonLayout* rowLayout = CommonLayout::create();
    rowLayout->setContentSize(
        Size(mainSize.width * 0.9, mainSize.height * 0.1));
    rowLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
    for (int j = 0; j < 4; j++) {
      int levelNumber = ((i * 4) + j) + startValue;
      Layout* lButton = createLevelButton(levelNumber);
      rowLayout->addChild(lButton);
    }
    rowLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
    gridLayout->addChild(rowLayout);
  }

  gridLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  mainLayout->addChild(gridLayout);
}

Layout* LevelSelect::createLevelButton(int levelNo) {
  using cocos2d::ui::ImageView;

  int starCount = GameManager::getInstance()->getLevelStars(levelNo);
  Button* levelButton = UiUtil::createButton(
      "button_orange.png", std::to_string(levelNo), 30, false);
  levelButtons.push_back(levelButton);
  Size levelButtonSize = levelButton->getContentSize();
  CommonLayout* buttonBg = CommonLayout::create();
  buttonBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  buttonBg->setContentSize(
      Size(levelButtonSize.width * 1.3, levelButtonSize.height * 1.3));
  buttonBg->setBackGroundImage("./cutout_tiny", Widget::TextureResType::PLIST);
  buttonBg->setBackGroundImageScale9Enabled(true);
  Size buttonSize = levelButton->getContentSize();
  CommonLayout* starsLayout = CommonLayout::create();
  starsLayout->setName("starsLayout");
  starsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  starsLayout->setContentSize(Size(buttonSize.width, buttonSize.height * 0.4));
  if (levelNo > GameManager::getInstance()->getCurrentLevel()) {
    levelButton->loadTextureNormal("button_locked.png",
                                   Widget::TextureResType::LOCAL);
    levelButton->setTitleText("");
  } else {
    for (int i = 1; i <= 3; i++) {
      ImageView* starImage = nullptr;
      if (starCount >= i) {
        starImage =
            ImageView::create("./star_on", Widget::TextureResType::PLIST);
      } else {
        starImage =
            ImageView::create("./star_off", Widget::TextureResType::PLIST);
      }
      starImage->setScale(0.30f);
      starsLayout->addChild(starImage);
    }
  }
  buttonBg->setScale(Config::DSP_SCALE);
  buttonBg->addChild(levelButton);
  if (levelNo > Config::TOTAL_LEVELS) {
    levelButton->setVisible(false);
  }
  starsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  buttonBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  starsLayout->setPositionNormalized(Vec2(0.0f, 1.0f));
  levelButton->addChild(starsLayout);
  levelButton->addTouchEventListener(
      CC_CALLBACK_2(LevelSelect::CBBtnLevel, this));
  return buttonBg;
}

void LevelSelect::CBBtnLevel(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    Button* buttonClicked = reinterpret_cast<Button*>(pSender);
    if (buttonClicked->getTitleText() != "") {
      int levelNo = stoi(buttonClicked->getTitleText());
      UiUtil::transitionFade(GameScene::createScene(levelNo));
    }
  }
}

void LevelSelect::CBBtnBack(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(MainMenu::createScene());
  }
}

void LevelSelect::CBPagePrevious(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    startValue -= 16;
    if (startValue == 1) {
      previousButton->setVisible(false);
    } else {
      if (!nextButton->isVisible()) nextButton->setVisible(true);
    }

    refreshLevelButtons();
  }
}

void LevelSelect::CBPageNext(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    startValue += 16;
    if (startValue == 625) {
      nextButton->setVisible(false);
    } else {
      if (!previousButton->isVisible()) previousButton->setVisible(true);
    }

    refreshLevelButtons();
  }
}

void LevelSelect::refreshLevelButtons() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int index = ((i * 4) + j);
      int levelText = ((i * 4) + j) + startValue;
      int starCount = GameManager::getInstance()->getLevelStars(levelText);
      Button* button = levelButtons.at(index);
      CommonLayout* starsLayout = reinterpret_cast<CommonLayout*>(
          button->getChildByName("starsLayout"));
      starsLayout->removeAllChildren();
      if (!button->isVisible()) button->setVisible(true);
      button->setTitleText(std::to_string(levelText));
      if (levelText <= Config::TOTAL_LEVELS) {
        if (levelText > GameManager::getInstance()->getCurrentLevel()) {
          button->loadTextureNormal("button_locked.png",
                                    Widget::TextureResType::LOCAL);
          button->setTitleText("");
        } else {
          button->loadTextureNormal("button_orange.png",
                                    Widget::TextureResType::LOCAL);
          button->setTitleText(std::to_string(levelText));
          for (int i = 1; i <= 3; i++) {
            ImageView* starImage = nullptr;
            if (starCount >= i) {
              starImage =
                  ImageView::create("./star_on", Widget::TextureResType::PLIST);
            } else {
              starImage = ImageView::create("./star_off",
                                            Widget::TextureResType::PLIST);
            }
            starImage->setScale(0.35f);
            starsLayout->addChild(starImage);
          }
        }
      } else {
        button->setVisible(false);
      }
    }
  }
}

void LevelSelect::addButtonsLayout() {
  Size mainSize = mainLayout->getContentSize();
  buttonsLayout = CommonLayout::create();
  buttonsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  buttonsLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.1));

  previousButton = UiUtil::createButton("button_page_arrow.png", "", 10, false);
  previousButton->addTouchEventListener(
      CC_CALLBACK_2(LevelSelect::CBPagePrevious, this));

  nextButton = UiUtil::createButton("button_page_arrow.png", "", 10, false);
  nextButton->addTouchEventListener(
      CC_CALLBACK_2(LevelSelect::CBPageNext, this));
  nextButton->setFlippedX(true);

  Size buttonSize = nextButton->getContentSize();

  CommonLayout* buttonBg_1 = CommonLayout::create();
  buttonBg_1->setLayoutType(CommonLayout::Type::HORIZONTAL);
  buttonBg_1->setContentSize(
      Size(buttonSize.width * 1.3, buttonSize.height * 1.3));
  buttonBg_1->setBackGroundImage("./cutout_tiny",
                                 Widget::TextureResType::PLIST);
  buttonBg_1->setBackGroundImageScale9Enabled(true);
  buttonBg_1->addChild(previousButton);
  buttonBg_1->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  CommonLayout* buttonBg_2 = CommonLayout::create();
  buttonBg_2->setLayoutType(CommonLayout::Type::HORIZONTAL);
  buttonBg_2->setContentSize(
      Size(buttonSize.width * 1.3, buttonSize.height * 1.3));
  buttonBg_2->setBackGroundImage("./cutout_tiny",
                                 Widget::TextureResType::PLIST);
  buttonBg_2->setBackGroundImageScale9Enabled(true);
  buttonBg_2->addChild(nextButton);
  buttonBg_2->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  buttonBg_1->setScale(Config::DSP_SCALE);
  buttonBg_2->setScale(Config::DSP_SCALE);
  buttonsLayout->addChild(buttonBg_1);
  buttonsLayout->addChild(buttonBg_2);

  buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  mainLayout->addChild(buttonsLayout);

  if (startValue == 1) {
    previousButton->setVisible(false);
  }
  if (startValue == 625) {
    nextButton->setVisible(false);
  }
}

}  // namespace scenes
