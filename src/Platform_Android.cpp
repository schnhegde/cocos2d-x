#include <jni.h>
#include <stdio.h>
#include <string.h>

#include "platform/android/jni/JniHelper.h"
// #include "client/linux/handler/exception_handler.h"
// #include "client/linux/handler/minidump_descriptor.h"

#include "data/Events.h"
// #include "data/Globals.h"
// #include "models/User.h"
#include "modules/EventUtils.h"

#define LOG_TAG "Platform_Android"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" {

void initAds() {
  cocos2d::JniMethodInfo methodInfo;

  if (!cocos2d::JniHelper::getStaticMethodInfo(
          methodInfo, "org/cocos2dx/cpp/AppActivity", "initAds", "()V"))
    return;

  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

bool isRewardedAdAvailable(string app) {
  cocos2d::JniMethodInfo methodInfo;
  bool ret = false;

  if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo,
                                               "org/cocos2dx/cpp/AppActivity",
                                               "isRewardedAdAvailable", "()Z"))
    return false;

  jstring appName = methodInfo.env->NewStringUTF(app.c_str());

  ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID,
                                                methodInfo.methodID, appName);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  return ret;
}

void showRewardedAd() {
  cocos2d::JniMethodInfo methodInfo;

  if (!cocos2d::JniHelper::getStaticMethodInfo(
          methodInfo, "org/cocos2dx/cpp/AppActivity", "showRewardedAd", "()V"))
    return;

  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void Java_org_cocos2dx_cpp_AppActivity_rewardVideoWatchCompleted() {
  // https://discuss.cocos2d-x.org/t/thread-safety-in-cocos2d-x/23557/3
  // https://stackoverflow.com/questions/28995470/how-to-support-multi-threading-in-cocos2d-x

  cocos2d::Director::getInstance()
      ->getScheduler()
      ->performFunctionInCocosThread([=]() {
        modules::EventUtils::dispatchEvent(
            data::Events::REWARDED_VIDEO_COMPLETED);
      });
}

void Java_org_cocos2dx_cpp_AppActivity_rewardVideoFailedToShow() {
  cocos2d::Director::getInstance()
      ->getScheduler()
      ->performFunctionInCocosThread([=]() {
        modules::EventUtils::dispatchEvent(data::Events::NO_VIDEOS);
      });
}

void Java_org_cocos2dx_cpp_AppActivity_rewardVideoCancelled() {
  cocos2d::Director::getInstance()
      ->getScheduler()
      ->performFunctionInCocosThread([=]() {
        modules::EventUtils::dispatchEvent(
            data::Events::REWARDED_VIDEO_CANCELLED);
      });
}
}
