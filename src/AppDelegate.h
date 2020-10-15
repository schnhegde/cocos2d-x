#ifndef SRC_APPDELEGATE_H_
#define SRC_APPDELEGATE_H_

#include "cocos2d.h"

class AppDelegate : private cocos2d::Application {
 public:
  AppDelegate();
  virtual ~AppDelegate();
  virtual void initGLContextAttrs();
  virtual bool applicationDidFinishLaunching();
  virtual void applicationDidEnterBackground();
  virtual void applicationWillEnterForeground();
};

#endif  // SRC_APPDELEGATE_H_
