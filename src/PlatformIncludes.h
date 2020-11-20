#ifndef SRC_PLATFORMINCLUDES_H_
#define SRC_PLATFORMINCLUDES_H_
#include <string>

#include "data/Config.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  // NOLINT
extern "C" void initAds();
extern "C" bool isRewardedAdAvailable();
extern "C" void showRewardedAd();
extern "C" void openFeedbackEmail();
extern "C" void purchase();
#endif

#endif  // SRC_PLATFORMINCLUDES_H_
