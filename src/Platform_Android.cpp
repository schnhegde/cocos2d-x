#include <jni.h>
#include <stdio.h>
#include <string>

#include "platform/android/jni/JniHelper.h"
// #include "client/linux/handler/exception_handler.h"
// #include "client/linux/handler/minidump_descriptor.h"

#include "data/Events.h"
// #include "data/Globals.h"
// #include "models/User.h"
#include "modules/EventUtils.h"
#include "modules/GameManager.h"
#include "modules/EventUtils.h"
#include "data/Events.h"
#include "cocos2d.h"

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

void openFeedbackEmail() {
  cocos2d::JniMethodInfo methodInfo;

  if (!cocos2d::JniHelper::getStaticMethodInfo(
          methodInfo, "org/cocos2dx/cpp/AppActivity", "openFeedbackEmail", "()V"))
    return;

  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void openReviewFlow() {
  cocos2d::JniMethodInfo methodInfo;

  if (!cocos2d::JniHelper::getStaticMethodInfo(
          methodInfo, "org/cocos2dx/cpp/AppActivity", "openReviewFlow", "()V"))
    return;

  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

bool isRewardedAdAvailable() {
  cocos2d::JniMethodInfo methodInfo;
  bool ret = false;

  if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo,
                                               "org/cocos2dx/cpp/AppActivity",
                                               "isRewardedAdAvailable", "()Z"))
    return false;

  ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID,
                                                methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  return ret;
}

std::string getVersionNumber() {
  cocos2d::JniMethodInfo methodInfo;

  jstring ret;
  if (!cocos2d::JniHelper::getStaticMethodInfo(
          methodInfo, "org/cocos2dx/cpp/AppActivity", "getVersionNumber",
          "()Ljava/lang/String;"))
    return nullptr;

  ret = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID,
                                                        methodInfo.methodID);
  methodInfo.env->DeleteLocalRef(methodInfo.classID);

  return cocos2d::JniHelper::jstring2string(ret);
}

bool isTablet() {
  cocos2d::JniMethodInfo methodInfo;
  bool ret = false;

  if (!cocos2d::JniHelper::getStaticMethodInfo(methodInfo,
                                               "org/cocos2dx/cpp/AppActivity",
                                               "isTablet", "()Z"))
    return false;

  ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID,
                                                methodInfo.methodID);
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

void purchase() {
  cocos2d::JniMethodInfo methodInfo;

  if (!cocos2d::JniHelper::getStaticMethodInfo(
          methodInfo, "org/cocos2dx/cpp/AppActivity", "purchase",
          "()V"))
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

void Java_org_cocos2dx_cpp_IAPPlugin_nativeConnectedEvent(JNIEnv *env,
                                                          jclass jclazz,
                                                          jboolean connected) {
  cocos2d::Director::getInstance()
      ->getScheduler()
      ->performFunctionInCocosThread([=]() {
        modules::GameManager::getInstance()->setBillingConnected(static_cast<bool>(connected));
      });
}

void Java_org_cocos2dx_cpp_IAPPlugin_nativePurchaseEvent(JNIEnv *env,
                                                          jclass jclazz,
                                                          jboolean purchased) {
  cocos2d::Director::getInstance()
      ->getScheduler()
      ->performFunctionInCocosThread([=]() {
        modules::GameManager::getInstance()->setGameOwned(static_cast<bool>(purchased));
        modules::EventUtils::dispatchEvent(data::Events::GAME_PURCHASED);
      });
}

void Java_org_cocos2dx_cpp_IAPPlugin_nativeOwnedEvent(JNIEnv *env,
                                                          jclass jclazz,
                                                          jboolean owned) {
  cocos2d::Director::getInstance()
      ->getScheduler()
      ->performFunctionInCocosThread([=]() {
        modules::GameManager::getInstance()->setGameOwned(static_cast<bool>(owned));
      });
}

}
