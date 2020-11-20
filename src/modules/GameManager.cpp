#include "./GameManager.h"

#include "../data/Keys.h"

using data::Keys;

namespace modules {

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::getInstance() {
  if (instance == nullptr) {
    instance = new GameManager();
    instance->init();
  }
  return instance;
}

void GameManager::init() {
  userPrefs = UserDefault::getInstance();
  totalStars = userPrefs->getIntegerForKey(Keys::TOTAL_STARS, 0);
  soundState = userPrefs->getBoolForKey(Keys::SOUND_STATE, true);
  musicState = userPrefs->getBoolForKey(Keys::MUSIC_STATE, true);
  undoCount = userPrefs->getIntegerForKey(Keys::UNDO_COUNT, 3);
  solutionCount = userPrefs->getIntegerForKey(Keys::SOLUTION_COUNT, 3);
  currentLevel = userPrefs->getIntegerForKey(Keys::CURRENT_LEVEL, 1);
  isTutorialDone = userPrefs->getBoolForKey(Keys::TUTORIAL_DONE, false);
  gameOwned = userPrefs->getBoolForKey(Keys::GAME_OWNED, false);
  billingConnected = false;
}

int GameManager::getUndoCount() { return undoCount; }

int GameManager::getSolutionCount() { return solutionCount; }

void GameManager::setUndoCount(int count) {
  undoCount = count;
  userPrefs->setIntegerForKey(Keys::UNDO_COUNT, undoCount);
}

void GameManager::setSolutionCount(int count) {
  solutionCount = count;
  userPrefs->setIntegerForKey(Keys::SOLUTION_COUNT, solutionCount);
}

bool GameManager::getSoundState() { return soundState; }

bool GameManager::getMusicState() { return musicState; }

void GameManager::setSoundState(bool state) {
  soundState = state;
  userPrefs->setBoolForKey(Keys::SOUND_STATE, soundState);
}

void GameManager::setLevelStars(int level, int stars) {
  char buff[100];
  sprintf(buff, Keys::LEVEL_STARS, level);
  int prevCount = userPrefs->getIntegerForKey(buff, 0);
  int diff = stars - prevCount;

  if (diff > 0) {
    totalStars += diff;
    userPrefs->setIntegerForKey(Keys::TOTAL_STARS, totalStars);
    userPrefs->setIntegerForKey(buff, stars);
    if (stars == 3) {
      // int chance = rand() % 100;
      // if (chance > 40) {
      //     return true;
      // }
    }
    // return false;
  } else {
    // return false;
  }
}

int GameManager::getLevelStars(int level) {
  char buff[100];
  sprintf(buff, Keys::LEVEL_STARS, level);
  return userPrefs->getIntegerForKey(buff, 0);
}

void GameManager::setCurrentLevel(int level) {
  currentLevel = level;
  userPrefs->setIntegerForKey(Keys::CURRENT_LEVEL, currentLevel);
}

int GameManager::getCurrentLevel() { return currentLevel; }

void GameManager::setTutorialDone(bool done) {
  isTutorialDone = done;
  userPrefs->setBoolForKey(Keys::TUTORIAL_DONE, isTutorialDone);
}

bool GameManager::getTutorialDone() { return isTutorialDone; }

void GameManager::setMusicState(bool state) {
  musicState = state;
  userPrefs->setBoolForKey(Keys::MUSIC_STATE, musicState);
}

int GameManager::getTotalStarsWon() { return totalStars; }

bool GameManager::isGameOwned() { return gameOwned; }
void GameManager::setGameOwned(bool owned) {
  gameOwned = owned;
  userPrefs->setBoolForKey(Keys::GAME_OWNED, gameOwned);
}

bool GameManager::getBillingConnected() { return billingConnected; }
void GameManager::setBillingConnected(bool connected) { billingConnected = connected; }

}  // namespace modules
