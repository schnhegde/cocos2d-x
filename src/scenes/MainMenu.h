#ifndef SRC_SCENES_MAINMENU_H_
#define SRC_SCENES_MAINMENU_H_

#include "../modules/CommonLayout.h"
#include "cocos2d.h"

using cocos2d::Scene;
using cocos2d::ui::Button;
using cocos2d::ui::ImageView;
using cocos2d::ui::Scale9Sprite;
using cocos2d::ui::Widget;
using modules::CommonLayout;

namespace scenes {

class MainMenu : public Scene {
 private:
  CommonLayout* mainLayout;

  CommonLayout* headerLayout;
  CommonLayout* logoLayout;
  CommonLayout* buttonsLayout;
  CommonLayout* versionLayout;

  Button* playButton;
  Button* settingsButton;

  void addHeaderLayout();
  void addLogoLayout();
  void addButtonsLayout();
  void addVersionLayout();

  void addGiveReviewPopup();

  void loadPlistFile();

  void CBBtnPrivacy(Ref* pSender, Widget::TouchEventType type);
  void CBBtnPlay(Ref* pSender, Widget::TouchEventType type);
  void CBBtnSettings(Ref* pSender, Widget::TouchEventType type);

 public:
  static Scene* createScene();
  virtual bool init();
  virtual void onEnter();
  CREATE_FUNC(MainMenu);
};

}  // namespace scenes

#endif  // SRC_SCENES_MAINMENU_H_
