#ifndef SRC_SCENES_SETTINGS_H_
#define SRC_SCENES_SETTINGS_H_

#include <vector>

#include "../modules/CommonLayout.h"
#include "cocos2d.h"

using cocos2d::Scene;
using cocos2d::ui::Button;
using cocos2d::ui::ImageView;
using cocos2d::ui::Scale9Sprite;
using cocos2d::ui::Widget;
using modules::CommonLayout;
using std::vector;

namespace scenes {

class Settings : public Scene {
 private:
  bool isPaused;

  CommonLayout* mainLayout;

  CommonLayout* headerLayout;
  CommonLayout* buttonsLayout;
  CommonLayout* messageLayout;

  Button* backButton;
  Button* playButton;
  Button* homeButton;
  Button* soundButton;
  Button* musicButton;
  Button* buyButton;
  Button* feedbackButton;

  void addHeaderLayout();
  void addMessageLayout();
  void addButtonsLayout();
  void loadPlistFile();
  void CBBtnBack(Ref* pSender, Widget::TouchEventType type);
  void CBBtnHome(Ref* pSender, Widget::TouchEventType type);
  void CBBtnPlay(Ref* pSender, Widget::TouchEventType type);
  void CBBtnSound(Ref* pSender, Widget::TouchEventType type);
  void CBBtnMusic(Ref* pSender, Widget::TouchEventType type);
  void CBBtnBuy(Ref* pSender, Widget::TouchEventType type);
  void CBBtnFeedback(Ref* pSender, Widget::TouchEventType type);

 public:
  static Scene* createScene(bool isPaused);
  virtual bool init();
  virtual void onEnter();
  CREATE_FUNC(Settings);
};

}  // namespace scenes

#endif  // SRC_SCENES_SETTINGS_H_
