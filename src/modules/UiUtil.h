#ifndef SRC_MODULES_UIUTIL_H_
#define SRC_MODULES_UIUTIL_H_

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <string>

#include "CommonLayout.h"

using std::string;

using cocos2d::ui::Button;

namespace modules {

class UiUtil {
 public:
  static Button* createButton(string textureName, string title,
                              int titleSize = 20, bool plist = true);
  static void transitionFade(cocos2d::Scene* scene);
  static void popFade(cocos2d::Node* root);
  static void pushFade(cocos2d::Scene* root, cocos2d::Scene* child);
  static void removeChild(cocos2d::Node* child);
};

}  // namespace modules

#endif  // SRC_MODULES_UIUTIL_H_
