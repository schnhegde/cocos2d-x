#ifndef SRC_MODULES_SOUNDUTIL_H_
#define SRC_MODULES_SOUNDUTIL_H_

#include <string>

#include "GameManager.h"
#include "audio/include/AudioEngine.h"

using cocos2d::experimental::AudioEngine;

namespace modules {

class SoundUtil {
 public:
  static SoundUtil* getInstance();
  void initSoundEngine();
  void preloadGameplayEffects();
  void unloadGameplayEffects();
  void playEfxBtnTouched();
  void playEfxGameWon();
  void playEfxGameLost();
  void playEfxWalk();
  void playEfxBoxPush();
  void playEfxBg();
  void updateIsSoundOff();
  void stopSound();

 private:
  bool isSoundOff = false;
  void playSound(std::string efxName);
  void playSoundRepeatedly(std::string efxName);
  static SoundUtil* sharedInstance;
};

}  // namespace modules

#endif  // SRC_MODULES_SOUNDUTIL_H_
