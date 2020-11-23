#ifndef SRC_SCENES_GAMESCENE_H_
#define SRC_SCENES_GAMESCENE_H_

#include <string>

#include "../data/Events.h"
#include "../modules/CommonLayout.h"
#include "../modules/EventListenerManager.h"
#include "../views/Grid.h"
#include "cocos2d.h"
#include "Settings.h"

using cocos2d::EventCustom;
using cocos2d::Scene;
using cocos2d::Sprite;
using cocos2d::ui::Button;
using cocos2d::ui::ImageView;
using cocos2d::ui::Scale9Sprite;
using cocos2d::ui::Widget;
using data::Events;
using modules::CommonLayout;
using modules::EventListenerManager;
using std::string;
using views::Grid;

namespace scenes {

class GameScene : public Scene, public EventListenerManager {
  enum class PossibleRewards {SOLUTION, UNDO, MOVES};

 private:
  bool isTutorial;
  static const char *messages[3];

  bool tutorial_level;
  int tutorial_move_count;
  int tutorial_curr_move;

  cocos2d::ui::Text* tutorial_text;

  string curr_move;
  int levelNo;
  int maxMoves;
  int originalMaxMoves;
  int moveCount;
  bool gameFinished;
  int sizeX;
  int sizeY;
  bool timerPaused;
  bool isUndoReward;
  Sprite* handSprite;
  string levelData;
  string move_text;
  Grid* grid;
  CommonLayout* mainLayout;
  GameScene::PossibleRewards currentPossibleReward;

  CommonLayout* headerLayout;
  CommonLayout* gameLayout;
  CommonLayout* buttonsLayout;
  Settings* pausePopup;

  cocos2d::EventListenerKeyboard* backButtonListener;

  Button* pauseButton;
  Button* solutionButton;
  Button* undoButton;
  Button* retryButton;

  cocos2d::ui::Text* movesText;

  // touch events
  bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
  void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
  void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
  void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
  bool isTouchDown;

  float initialTouchPos[2];
  float currentTouchPos[2];

  void addBackButtonListener();
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

  void addHeaderLayout();
  void addGameLayout();
  void addButtonsLayout();

  void loadPlistFile();

  void CBBtnPause(Ref* pSender, Widget::TouchEventType type);
  void CBBtnSolution(Ref* pSender, Widget::TouchEventType type);
  void CBBtnUndo(Ref* pSender, Widget::TouchEventType type);
  void CBBtnRetry(Ref* pSender, Widget::TouchEventType type);

  void pauseClosed(EventCustom* event);
  void handleGameComplete(EventCustom* event);
  void handleMoveCompleted(EventCustom* event);
  void handleGameResumed(EventCustom* event);
  void handleUndoCompleted(EventCustom* event);
  void handleAdsShowing(EventCustom* event);
  void handleAdsClosed(EventCustom* event);
  void rewardedCompleted(EventCustom* event);
  void rewardedCancelled(EventCustom* event);
  void CBBtnUp(Ref* target);
  void CBBtnDown(Ref* target);
  void CBBtnLeft(Ref* target);
  void CBBtnRight(Ref* target);
  void update(float dt);
  void startTutorial();
  void nextMove();
  void showMove(const char move);
  void animateHand(int x, int y);
  void refreshButtons();

 public:
  static Scene* createScene(int levelNo, bool isTutorial = false);
  virtual bool init();
  virtual void initEventStrings();
  virtual void initEventHandlers();
  virtual void onEnter();
  CREATE_FUNC(GameScene);
};

}  // namespace scenes

#endif  // SRC_SCENES_GAMESCENE_H_
