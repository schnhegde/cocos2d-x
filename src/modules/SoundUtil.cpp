#include "SoundUtil.h"

#include "../data/Config.h"

using data::Config;

namespace modules {

SoundUtil* SoundUtil::sharedInstance = nullptr;

SoundUtil* SoundUtil::getInstance() {
  if (sharedInstance == nullptr) {
    sharedInstance = new SoundUtil();
    sharedInstance->initSoundEngine();
    sharedInstance->updateIsSoundOff();
  }
  return sharedInstance;
}

void SoundUtil::initSoundEngine() { preloadGameplayEffects(); }

void SoundUtil::preloadGameplayEffects() {
  AudioEngine::preload(Config::EFX_BTN_TOUCH);
  //  soundEngine->preloadBackgroundMusic(Config::EFX_BG);
}

void SoundUtil::unloadGameplayEffects() { AudioEngine::uncacheAll(); }

void SoundUtil::playEfxBtnTouched() { playSound(Config::EFX_BTN_TOUCH); }

void SoundUtil::playEfxGameWon() { /*playSound(Config::EFX_GAME_WIN);*/
}
void SoundUtil::playEfxGameLost() { /*playSound(Config::EFX_GAME_LOST);*/
}
void SoundUtil::playEfxWalk() {
  // playSound(Config::EFX_WALK);
}
void SoundUtil::playEfxBoxPush() {
  // playSound(Config::EFX_BOX_PUSH);
}
void SoundUtil::updateIsSoundOff() {
  isSoundOff = !GameManager::getInstance()->getSoundState();
  if (isSoundOff) stopSound();
}

void SoundUtil::playSound(std::string efxName) {
  if (!isSoundOff) {
    AudioEngine::play2d(efxName.c_str(), false, 0.7f);
  }
}

void SoundUtil::playEfxBg() {
  //  playSoundRepeatedly(Config::EFX_BG);
}

void SoundUtil::playSoundRepeatedly(std::string efxName) {
  if (!isSoundOff) AudioEngine::play2d(efxName.c_str(), true, 0.3f);
}

void SoundUtil::stopSound() { AudioEngine::stopAll(); }

}  // namespace modules
