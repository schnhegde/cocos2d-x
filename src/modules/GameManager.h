#ifndef SRC_MODULES_GAMEMANAGER_H_
#define SRC_MODULES_GAMEMANAGER_H_

#include "cocos2d.h"

using cocos2d::UserDefault;

namespace modules {

class GameManager {
 private:
  static GameManager* instance;
  UserDefault* userPrefs;

  int undoCount;
  int solutionCount;
  bool soundState;
  bool musicState;
  int currentLevel;
  int totalStars;
  bool isTutorialDone;
  void init();
  bool billingConnected;
  bool gameOwned;

 public:
  static GameManager* getInstance();
  bool getSoundState();
  bool getMusicState();
  void setSoundState(bool state);
  void setMusicState(bool state);
  int getTotalStarsWon();

  int getCurrentLevel();
  void setCurrentLevel(int level);

  int getLevelStars(int level);
  void setLevelStars(int level, int stars);

  int getSolutionCount();
  void setSolutionCount(int count);

  int getUndoCount();
  void setUndoCount(int count);

  void setTutorialDone(bool done);
  bool getTutorialDone();

  void setBillingConnected(bool connected);
  bool getBillingConnected();

  void setGameOwned(bool owned);
  bool isGameOwned();
};

}  // namespace modules

#endif  // SRC_MODULES_GAMEMANAGER_H_
