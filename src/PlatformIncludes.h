#ifndef SRC_PLATFORMINCLUDES_H_
#define SRC_PLATFORMINCLUDES_H_
#include <string>

#include "data/Config.h"
using std::string;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  // NOLINT
extern "C" void initAds();
extern "C" void isRewardedAdLoaded();
extern "C" void showRewardedAd();
#endif

#endif  // SRC_PLATFORMINCLUDES_H_
