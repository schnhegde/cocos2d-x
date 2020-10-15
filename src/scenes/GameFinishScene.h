#ifndef SRC_SCENES_GAMEFINISHSCENE_H_
#define SRC_SCENES_GAMEFINISHSCENE_H_

#include "../modules/CommonLayout.h"
#include "cocos2d.h"

using cocos2d::Ref;
using cocos2d::Scene;
using cocos2d::ui::Button;
using cocos2d::ui::Widget;
using modules::CommonLayout;

namespace scenes {

class GameFinishScene : public Scene {
 private:
  int starCount;
  int movesMade;

  CommonLayout* mainLayout;

  CommonLayout* headerLayout;
  CommonLayout* buttonsLayout;
  CommonLayout* detailLayout;

  Button* levelSelectButton;
  Button* playButton;
  Button* homeButton;

  void addHeaderLayout();
  void addDetailLayout();
  void addButtonsLayout();
  void loadPlistFile();
  void CBBtnHome(Ref* pSender, Widget::TouchEventType type);
  void CBBtnPlay(Ref* pSender, Widget::TouchEventType type);
  void CBBtnLevelSelect(Ref* pSender, Widget::TouchEventType type);

 public:
  static Scene* createScene(int movesMade, int starCount = 0);
  virtual bool init();
  virtual void onEnter();
  CREATE_FUNC(GameFinishScene);
};

}  // namespace scenes

#endif  // SRC_SCENES_GAMEFINISHSCENE_H_
