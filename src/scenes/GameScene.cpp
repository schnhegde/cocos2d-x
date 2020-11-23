#include "GameScene.h"

#include <string>

#include "../data/Config.h"
#include "../modules/EventUtils.h"
#include "../modules/GameManager.h"
#include "../modules/LevelLoader.h"
#include "../modules/UiUtil.h"
#include "../modules/SoundUtil.h"
#include "./LevelSelect.h"
#include "./Settings.h"
#include "GameFinishScene.h"
#include "WatchVideoPopup.h"
#include "MessagePopup.h"
#include "OutOfMovesPopup.h"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Event;
using cocos2d::EventCustom;
using cocos2d::EventListenerTouchOneByOne;
using cocos2d::Size;
using cocos2d::SpriteFrameCache;
using cocos2d::TextHAlignment;
using cocos2d::TextVAlignment;
using cocos2d::Touch;
using cocos2d::Vec2;
using cocos2d::ui::Text;
using cocos2d::ui::Widget;
using data::Config;
using modules::EventUtils;
using modules::GameManager;
using modules::LevelLoader;
using modules::UiUtil;
using scenes::GameFinishScene;
using std::string;
using modules::SoundUtil;

namespace scenes {

const char* GameScene::messages[3] = { "Swipe right to complete the level", "Swipe up/down/left/right\nto move the worker",
                               "Push the boxes to their goals to win!"};

Scene* GameScene::createScene(int levelNo, bool isTutorial) {
  auto scene = GameScene::create();
  scene->levelNo = levelNo;
  scene->isTutorial = isTutorial;
  return scene;
}

static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf(
      "Depending on how you compiled you might have to add 'Resources/' in "
      "front of filenames in GameScene.cpp\n");
}

bool GameScene::init() {
  if (!Scene::init()) {
    return false;
  }
  currentPossibleReward = GameScene::PossibleRewards::UNDO;
  loadPlistFile();
  initListeners(this);
  return true;
}

void GameScene::addBackButtonListener() {
  backButtonListener = cocos2d::EventListenerKeyboard::create();
  backButtonListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
  cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButtonListener, this);
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
  if(keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACK) {
    CBBtnPause(nullptr, Widget::TouchEventType::ENDED);
  }
}

void GameScene::loadPlistFile() {
  SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
  frameCache->addSpriteFramesWithFile("GameScene.plist");
}

void GameScene::initEventStrings() {
  EventListenerManager::eventStrs.push_back(Events::GAME_COMPLETED);
  EventListenerManager::eventStrs.push_back(Events::GAME_MOVE_COMPLETED);
  EventListenerManager::eventStrs.push_back(Events::GAME_RESUMED);
  EventListenerManager::eventStrs.push_back(Events::GAME_UNDO_COMPLETED);
  EventListenerManager::eventStrs.push_back(Events::ADS_SHOWING);
  EventListenerManager::eventStrs.push_back(Events::ADS_CLOSED);
  EventListenerManager::eventStrs.push_back(Events::REWARDED_VIDEO_COMPLETED);
  EventListenerManager::eventStrs.push_back(Events::REWARDED_VIDEO_CANCELLED);
  EventListenerManager::eventStrs.push_back(Events::PAUSE_CLOSED);
}

void GameScene::initEventHandlers() {
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::handleGameComplete, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::handleMoveCompleted, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::handleGameResumed, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::handleUndoCompleted, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::handleAdsShowing, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::handleAdsClosed, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::rewardedCompleted, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::rewardedCancelled, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(GameScene::pauseClosed, this));
}

void GameScene::pauseClosed(EventCustom* event) {
  if (pausePopup != nullptr) {
    pausePopup->removeFromParent();
    pausePopup = nullptr;
    addBackButtonListener();
  }
}

void GameScene::rewardedCompleted(EventCustom* event) {
  string messageString = "";
  GameManager* gm = GameManager::getInstance();
  int solutionCount = gm->getSolutionCount();
  int undoCount = gm->getUndoCount();

  switch (currentPossibleReward) {
    case PossibleRewards::MOVES:
      messageString = "Hurray! you got +";
      messageString += std::to_string(originalMaxMoves);
      messageString += " moves";
      maxMoves = originalMaxMoves;
      movesText->setString(std::to_string(maxMoves));
      break;
    case PossibleRewards::SOLUTION:
      messageString = "Hurray! you got +1 solution";
      solutionCount++;
      gm->setSolutionCount(solutionCount);
    break;
    default:
    case PossibleRewards::UNDO:
      messageString = "Hurray! you got +5 undo";
      undoCount += Config::REWARDED_UNDO_COUNT;
      gm->setUndoCount(undoCount);
    break;
  }
  refreshButtons();
  auto messagePopup = MessagePopup::createPopup(messageString);
  addChild(messagePopup, 10);
}

void GameScene::refreshButtons() {
  GameManager* gm = GameManager::getInstance();
  int solutionCount = gm->getSolutionCount();
  int undoCount = gm->getUndoCount();
  undoButton->getTitleRenderer()->setString(std::to_string(undoCount));
  solutionButton->getTitleRenderer()->setString(std::to_string(solutionCount));
}

void GameScene::rewardedCancelled(EventCustom* event) {
  if (currentPossibleReward == GameScene::PossibleRewards::MOVES) {
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(backButtonListener);
    gameFinished = true;
    addChild(GameFinishScene::createScene(moveCount, levelNo), 10);
  } else {
    string messageString = "Oops! watch the full video for rewards";
    auto messagePopup = MessagePopup::createPopup(messageString);
    addChild(messagePopup, 10);
  }
}

void GameScene::handleGameComplete(EventCustom* event) {
  int nextLevel = levelNo + 1;
  if (nextLevel >= Config::TOTAL_LEVELS) nextLevel = Config::TOTAL_LEVELS;
  auto gsm = GameManager::getInstance();
  int starCount = 0;
  int auto_move_count = LevelLoader::getLevelTotalMoves(levelNo);
  bool isReward = false;
  if (moveCount <= auto_move_count) {
    gsm->setLevelStars(levelNo, 3);
    starCount = 3;
  } else if (moveCount <=
             auto_move_count +
                 std::ceil(static_cast<float>(auto_move_count) * 0.40)) {
    gsm->setLevelStars(levelNo, 2);
    starCount = 2;
  } else {
    gsm->setLevelStars(levelNo, 1);
    starCount = 1;
  }
  int currLevel = gsm->getCurrentLevel();
  if (nextLevel > currLevel) {
    gsm->setCurrentLevel(nextLevel);
  }
  // unScheduleGameTimer();
  cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(backButtonListener);
  gameFinished = true;
  addChild(GameFinishScene::createScene(moveCount, levelNo, starCount), 10);
}

void GameScene::handleMoveCompleted(EventCustom* event) {
  moveCount++;
  maxMoves--;
  if (maxMoves <= 0) {
    // show game failed popup
    //        unScheduleGameTimer();
    // 
    currentPossibleReward = GameScene::PossibleRewards::MOVES;
    auto popup = OutOfMovesPopup::createPopup(originalMaxMoves);
    addChild(popup, 10);
  }
  movesText->setString(std::to_string(maxMoves));
}

void GameScene::handleGameResumed(EventCustom* event) {}

void GameScene::handleUndoCompleted(EventCustom* event) {
  int undoCount = GameManager::getInstance()->getUndoCount();
  undoCount--;
  GameManager::getInstance()->setUndoCount(undoCount);
  string undoString = std::to_string(undoCount);
  undoButton->getTitleRenderer()->setString(undoString);
}

void GameScene::handleAdsShowing(EventCustom* event) {}

void GameScene::handleAdsClosed(EventCustom* event) {}

void GameScene::onEnter() {
  Scene::onEnter();
  pausePopup = nullptr;
  addBackButtonListener();
  gameFinished = false;
  moveCount = 0;
  levelData = LevelLoader::getLevelData(levelNo);
  sizeX = LevelLoader::getLevelSizeX(levelNo);
  sizeY = LevelLoader::getLevelSizeY(levelNo);
  move_text = LevelLoader::getLevelSolution(levelNo);
  int best_move_size = move_text.length();
  maxMoves =
      best_move_size + std::ceil(static_cast<float>(best_move_size) * 0.5);
  originalMaxMoves = maxMoves;
  Size screenSize = Director::getInstance()->getVisibleSize();
  mainLayout = CommonLayout::create();
  mainLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  mainLayout->setContentSize(screenSize);
  mainLayout->setBackGroundImage("./bg", Widget::TextureResType::PLIST);
  mainLayout->setBackGroundImageScale9Enabled(true);
  addHeaderLayout();
  addGameLayout();
  addButtonsLayout();
  mainLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  addChild(mainLayout);

  auto listener = EventListenerTouchOneByOne::create();
  listener->setSwallowTouches(true);

  listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
  listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
  listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
  listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);

  Director::getInstance()
      ->getEventDispatcher()
      ->addEventListenerWithSceneGraphPriority(listener, this);

  isTouchDown = false;

  initialTouchPos[0] = 0;
  initialTouchPos[1] = 0;
  this->scheduleUpdate();

  int current_max_level = GameManager::getInstance()->getCurrentLevel();
  if (isTutorial || current_max_level <= 3) {
    if (current_max_level <= 3) {
      std::string messageText = messages[current_max_level - 1];
      tutorial_text = Text::create(messageText, Config::FONT_FILE, 30);
      tutorial_text->enableOutline(cocos2d::Color4B::BLACK, 1);
      tutorial_text->setPositionNormalized(cocos2d::Vec2(0.5, 0.2));
      tutorial_text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
      tutorial_text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
      addChild(tutorial_text, 10);
    }
    startTutorial();
  }
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
  initialTouchPos[0] = touch->getLocation().x;
  initialTouchPos[1] = touch->getLocation().y;
  currentTouchPos[0] = touch->getLocation().x;
  currentTouchPos[1] = touch->getLocation().y;

  isTouchDown = true;

  return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
  currentTouchPos[0] = touch->getLocation().x;
  currentTouchPos[1] = touch->getLocation().y;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
  isTouchDown = false;
}

void GameScene::onTouchCancelled(Touch* touch, Event* event) {
  onTouchEnded(touch, event);
}

void GameScene::update(float dt) {
  Size visibleSize = Director::getInstance()->getVisibleSize();
  if (true == isTouchDown) {
    float dx = std::abs(initialTouchPos[0] - currentTouchPos[0]);
    float dy = std::abs(initialTouchPos[1] - currentTouchPos[1]);
    float delta = std::abs(dx - dy);
    if (dx > dy) {
      if (initialTouchPos[0] - currentTouchPos[0] > visibleSize.width * 0.03) {
        if (tutorial_level) {
          if (curr_move == "l") {
            tutorial_curr_move += 1;
            nextMove();
            CBBtnLeft(nullptr);
            isTouchDown = false;
          }

        } else {
          CBBtnLeft(nullptr);
          isTouchDown = false;
        }

      } else if (initialTouchPos[0] - currentTouchPos[0] <
                 -visibleSize.width * 0.03) {
        if (tutorial_level) {
          if (curr_move == "r") {
            tutorial_curr_move += 1;
            nextMove();
            CBBtnRight(nullptr);
            isTouchDown = false;
          }

        } else {
          CBBtnRight(nullptr);
          isTouchDown = false;
        }
      }
    } else {
      if (initialTouchPos[1] - currentTouchPos[1] > visibleSize.width * 0.03) {
        if (tutorial_level) {
          if (curr_move == "d") {
            tutorial_curr_move += 1;
            nextMove();
            CBBtnDown(nullptr);
            isTouchDown = false;
          }

        } else {
          CBBtnDown(nullptr);
          isTouchDown = false;
        }
      } else if (initialTouchPos[1] - currentTouchPos[1] <
                 -visibleSize.width * 0.03) {
        if (tutorial_level) {
          if (curr_move == "u") {
            tutorial_curr_move += 1;
            nextMove();
            CBBtnUp(nullptr);
            isTouchDown = false;
          }

        } else {
          CBBtnUp(nullptr);
          isTouchDown = false;
        }
      }
    }
  }
}

void GameScene::CBBtnUp(Ref* target) { grid->movePlayer(Vec2(0, -1)); }

void GameScene::CBBtnRight(Ref* target) { grid->movePlayer(Vec2(1, 0)); }

void GameScene::CBBtnLeft(Ref* target) { grid->movePlayer(Vec2(-1, 0)); }

void GameScene::CBBtnDown(Ref* target) { grid->movePlayer(Vec2(0, 1)); }

void GameScene::addHeaderLayout() {
  using cocos2d::ui::Margin;
  Size mainSize = mainLayout->getContentSize();
  headerLayout = CommonLayout::create();
  headerLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  headerLayout->setContentSize(
      Size(mainSize.width * 0.85, mainSize.height * 0.15));
  headerLayout->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  headerLayout->setBackGroundImageScale9Enabled(true);

  Size headerSize = headerLayout->getContentSize();

  CommonLayout* movesLayout = CommonLayout::create();
  movesLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  movesLayout->setContentSize(
      Size(headerSize.height * 0.8, headerSize.height * 0.8));
  Text* movesLeftLabel = Text::create("Moves Left", Config::FONT_FILE, 28);
  movesLeftLabel->enableOutline(Color4B::BLACK, 1);
  CommonLayout* movesLeftLayout = CommonLayout::create();
  movesLeftLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  movesLeftLayout->setContentSize(
      Size(headerSize.height * 0.5, headerSize.height * 0.5));
  movesLeftLayout->setBackGroundImage("./dark_bg_tiny",
                                      Widget::TextureResType::PLIST);
  movesLeftLayout->setBackGroundImageScale9Enabled(true);
  movesText = Text::create(std::to_string(maxMoves), Config::FONT_FILE, 30);
  movesText->enableOutline(Color4B::BLACK, 1);
  movesLeftLayout->addChild(movesText);
  movesLeftLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  movesLayout->addChild(movesLeftLabel);
  movesLayout->addChild(movesLeftLayout);
  movesLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  headerLayout->addChild(movesLayout);
  string levelString = "Level ";
  levelString += std::to_string(levelNo);
  Text* levelText = Text::create(levelString, Config::FONT_FILE, 42);
  levelText->enableOutline(Color4B::BLACK, 1);
  headerLayout->addChild(levelText);

  pauseButton = UiUtil::createButton("./button_pause", "");
  pauseButton->addTouchEventListener(
      CC_CALLBACK_2(GameScene::CBBtnPause, this));
  headerLayout->addChild(pauseButton);

  headerLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  headerLayout->setScale(Config::DSP_SCALE);
  mainLayout->addChild(headerLayout);
}

void GameScene::addGameLayout() {
  Size mainSize = mainLayout->getContentSize();
  gameLayout = CommonLayout::create();
  gameLayout->setLayoutType(CommonLayout::Type::VERTICAL);
  gameLayout->setContentSize(
      Size(mainSize.width * 0.95, mainSize.height * 0.63));
  //    gameLayout->setBackGroundImage("./cutout",
  //    Widget::TextureResType::PLIST);
  //    gameLayout->setBackGroundImageScale9Enabled(true);
  cocos2d::ui::ImageView* gameBg =
      cocos2d::ui::ImageView::create("./cutout", Widget::TextureResType::PLIST);

  grid = Grid::createGrid(levelData, sizeX, sizeY);
  Size grid_size = Size(Config::TILE_SIZE * sizeX + (Config::TILE_SIZE / 3),
                        Config::TILE_SIZE * sizeY + (Config::TILE_SIZE / 2));
  gameBg->setContentSize(Size(grid_size.width + Config::TILE_SIZE,
                              grid_size.height + Config::TILE_SIZE / 2));
  this->addChild(grid, 7);
  Size gameBgSize = gameBg->getContentSize();
  gameBg->setScale9Enabled(true);
  grid->setPosition(Vec2(mainSize.width / 2, (mainSize.height / 2) - (Config::TILE_SIZE)));
  gameBg->setPosition(Vec2(mainSize.width / 2, mainSize.height / 2));
  this->addChild(gameBg, 2);
  gameLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  mainLayout->addChild(gameLayout);
}

void GameScene::addButtonsLayout() {
  Size mainSize = mainLayout->getContentSize();
  buttonsLayout = CommonLayout::create();
  buttonsLayout->setLayoutType(CommonLayout::Type::HORIZONTAL);
  buttonsLayout->setContentSize(Size(mainSize.width, mainSize.height * 0.1));
  buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  int solutionCount = GameManager::getInstance()->getSolutionCount();
  solutionButton = UiUtil::createButton("./button_solution", "");
  solutionButton->addTouchEventListener(
      CC_CALLBACK_2(GameScene::CBBtnSolution, this));
  solutionButton->setTitleText(std::to_string(solutionCount));
  solutionButton->setTitleFontName(Config::FONT_FILE);
  solutionButton->setTitleFontSize(40);
  solutionButton->getTitleRenderer()->setTextColor(Color4B::YELLOW);
  solutionButton->getTitleRenderer()->enableOutline(Color4B::BLACK);
  solutionButton->getTitleRenderer()->setPositionNormalized(Vec2(0.85f, 0.85f));
  Size solutionSize = solutionButton->getContentSize();
  CommonLayout* solutionBg = CommonLayout::create();
  solutionBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  solutionBg->setContentSize(
      Size(solutionSize.width * 1.3, solutionSize.height * 1.3));
  solutionBg->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  solutionBg->setBackGroundImageScale9Enabled(true);
  solutionBg->addChild(solutionButton);
  solutionBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  int undoCount = GameManager::getInstance()->getUndoCount();
  undoButton = UiUtil::createButton("./button_undo", "");
  undoButton->addTouchEventListener(CC_CALLBACK_2(GameScene::CBBtnUndo, this));
  Size undoSize = solutionButton->getContentSize();
  undoButton->setTitleText(std::to_string(undoCount));
  undoButton->setTitleFontName(Config::FONT_FILE);
  undoButton->setTitleFontSize(40);
  undoButton->getTitleRenderer()->setTextColor(Color4B::YELLOW);
  undoButton->getTitleRenderer()->enableOutline(Color4B::BLACK);
  undoButton->getTitleRenderer()->setPositionNormalized(Vec2(0.85f, 0.85f));
  CommonLayout* undoBg = CommonLayout::create();
  undoBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  undoBg->setContentSize(Size(undoSize.width * 1.3, undoSize.height * 1.3));
  undoBg->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  undoBg->setBackGroundImageScale9Enabled(true);
  undoBg->addChild(undoButton);
  undoBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  retryButton = UiUtil::createButton("retry_button.png", "", 10, false);
  retryButton->addTouchEventListener(CC_CALLBACK_2(GameScene::CBBtnRetry, this));
  Size retrySize = retryButton->getContentSize();
  CommonLayout* retryBg = CommonLayout::create();
  retryBg->setLayoutType(CommonLayout::Type::HORIZONTAL);
  retryBg->setContentSize(Size(undoSize.width * 1.3, undoSize.height * 1.3));
  retryBg->setBackGroundImage("./cutout", Widget::TextureResType::PLIST);
  retryBg->setBackGroundImageScale9Enabled(true);
  retryBg->addChild(retryButton);
  retryBg->justifyChildren(CommonLayout::JUSTIFY::EVENLY);

  solutionBg->setScale(Config::DSP_SCALE);
  undoBg->setScale(Config::DSP_SCALE);
  retryBg->setScale(Config::DSP_SCALE);
  buttonsLayout->addChild(solutionBg);
  buttonsLayout->addChild(undoBg);
  buttonsLayout->addChild(retryBg);
  buttonsLayout->justifyChildren(CommonLayout::JUSTIFY::EVENLY);
  mainLayout->addChild(buttonsLayout);
}

void GameScene::startTutorial() {
  undoButton->setVisible(false);
  solutionButton->setVisible(false);
  tutorial_level = true;
  curr_move = "";
  tutorial_move_count = move_text.size();
  tutorial_curr_move = 0;
  nextMove();
}

void GameScene::nextMove() {
  if (tutorial_curr_move < tutorial_move_count) {
    showMove(move_text.at(tutorial_curr_move));
  }
}

void GameScene::showMove(const char move) {
  switch (move) {
    case 'l':
    case 'L':
      curr_move = "l";
      if (handSprite) handSprite->removeFromParent();
      animateHand(-1, 0);
      break;
    case 'u':
    case 'U':
      curr_move = "u";

      if (handSprite) handSprite->removeFromParent();
      animateHand(0, 1);
      break;
    case 'r':
    case 'R':
      curr_move = "r";
      if (handSprite) handSprite->removeFromParent();
      animateHand(1, 0);
      break;
    case 'd':
    case 'D':
      curr_move = "d";
      if (handSprite) handSprite->removeFromParent();
      animateHand(0, -1);
      break;
    default:
      return;
  }
}

void GameScene::animateHand(int x, int y) {
  handSprite = Sprite::create("hand.png");
  handSprite->setScale(Config::DSP_SCALE);
  Size visibleSize = Director::getInstance()->getVisibleSize();
  int startX = visibleSize.width / 2 - ((x * visibleSize.width) / 4);
  int startY = visibleSize.height / 2 - ((y * visibleSize.height) / 6);
  int endX = visibleSize.width / 2 + ((x * visibleSize.width) / 4);
  int endY = visibleSize.height / 2 + ((y * visibleSize.height) / 6);
  handSprite->setPosition(Vec2(startX, startY));
  addChild(handSprite, 8);
  auto moveAction = cocos2d::MoveTo::create(1.5, Vec2(endX, endY));
  auto scaleAction = cocos2d::ScaleTo::create(0.1, 0);
  auto moveBackAction = cocos2d::MoveTo::create(0.0, Vec2(startX, startY));
  auto scaleBackAction = cocos2d::ScaleTo::create(0.1, Config::DSP_SCALE);
  //    auto setPos =
  //    cocos2d::CallFunc::create(CC_CALLBACK_0(Sprite::setPosition, spr,
  //    Vec2(startX, startY)));
  //    cocos2d::RepeatForever::create(cocos2d::Sequence::create(moveAction,
  //    setPos, nullptr));
  auto final_action = cocos2d::RepeatForever::create(cocos2d::Sequence::create(
      moveAction, scaleAction, moveBackAction, scaleBackAction, nullptr));
  handSprite->runAction(final_action);
}

void GameScene::CBBtnUndo(Ref* sender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    int undoCount = GameManager::getInstance()->getUndoCount();

    if (undoCount > 0) {
      EventUtils::dispatchEvent(Events::GAME_UNDO);
    } else {
      if (!grid->checkIfUndo()) {
        // TODO(sachin): show rewarded popup
        timerPaused = true;
        isUndoReward = true;
        currentPossibleReward = GameScene::PossibleRewards::UNDO;
        auto popup = WatchVideoPopup::createPopup("undo", 5);
        addChild(popup, 10);
      }
    }
  }
}

void GameScene::CBBtnRetry(Ref* sender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    UiUtil::transitionFade(GameScene::createScene(levelNo));
  }
}

void GameScene::CBBtnPause(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(backButtonListener);
    pausePopup = (Settings*)Settings::createScene(true);
    UiUtil::pushFade(this, pausePopup);
  }
}

void GameScene::CBBtnSolution(Ref* pSender, Widget::TouchEventType type) {
  if (type == Widget::TouchEventType::ENDED) {
    SoundUtil::getInstance()->playEfxBtnTouched();
    int solutionCount = GameManager::getInstance()->getSolutionCount();
    if (solutionCount <= 0) {
      // show buy hints popup
      //        addChild(BuyHintsPopup::createPopup(), 10);
      isUndoReward = false;
      currentPossibleReward = GameScene::PossibleRewards::SOLUTION;
      auto popup = WatchVideoPopup::createPopup("solution");
      addChild(popup, 10);
    } else {
      solutionCount--;
      GameManager::getInstance()->setSolutionCount(solutionCount);
      Director::getInstance()->replaceScene(
          GameScene::createScene(levelNo, true));
    }
  }
}

}  // namespace scenes
