#include "AppDelegate.h"
#include "PlatformIncludes.h"
#include "modules/LevelLoader.h"
#include "modules/SoundUtil.h"
#include "scenes/SplashScene.h"
#include "scenes/MainMenu.h"
#include "data/Config.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using cocos2d::experimental::AudioEngine;
#endif

using cocos2d::Director;
using cocos2d::FileUtils;
using cocos2d::GLView;
using cocos2d::GLViewImpl;
using cocos2d::Size;
using modules::LevelLoader;
using modules::SoundUtil;
using scenes::SplashScene;
using scenes::MainMenu;

static Size designResolutionSize = Size(640, 960);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
  AudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs() {
  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
  GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
  return 0;  // flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if (!glview) {
    glview = GLViewImpl::create("Sokoban");
    director->setOpenGLView(glview);
  }

  director->setDisplayStats(false);
  director->setAnimationInterval(1.0f / 60);
  glview->setDesignResolutionSize(designResolutionSize.width,
                                  designResolutionSize.height,
                                  ResolutionPolicy::FIXED_WIDTH);
  register_all_packages();
  if (isTablet()) {
    data::Config::DSP_SCALE = 0.7f;
    data::Config::TILE_SIZE = data::Config::TILE_SIZE * data::Config::DSP_SCALE;
  }
  FileUtils::getInstance()->addSearchPath("images");
  FileUtils::getInstance()->addSearchPath("audio");
  FileUtils::getInstance()->addSearchPath("fonts");

  SoundUtil::getInstance();
  LevelLoader::loadLevels();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  auto scene = SplashScene::createScene();
  director->runWithScene(scene);
#else
  auto scene = MainMenu::createScene();
  director->runWithScene(scene);
#endif

  return true;
}

void AppDelegate::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
  AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
  AudioEngine::resumeAll();
#endif
}
