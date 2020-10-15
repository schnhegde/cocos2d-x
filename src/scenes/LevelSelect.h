#ifndef SRC_SCENES_LEVELSELECT_H_
#define SRC_SCENES_LEVELSELECT_H_

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

class LevelSelect : public Scene {
 private:
  int startValue;
  CommonLayout* mainLayout;

  CommonLayout* headerLayout;
  CommonLayout* gridLayout;
  CommonLayout* buttonsLayout;

  Button* backButton;
  Button* nextButton;
  Button* previousButton;
  vector<Button*> levelButtons;

  void addHeaderLayout();
  void addGridLayout();
  void addButtonsLayout();
  void refreshLevelButtons();
  Layout* createLevelButton(int levelNo);
  void CBBtnLevel(Ref* pSender, Widget::TouchEventType type);
  void CBBtnBack(Ref* pSender, Widget::TouchEventType type);

  void CBPageNext(Ref* pSender, Widget::TouchEventType type);
  void CBPagePrevious(Ref* pSender, Widget::TouchEventType type);

 public:
  static Scene* createScene();
  virtual bool init();
  virtual void onEnter();
  CREATE_FUNC(LevelSelect);
};

}  // namespace scenes

#endif  // SRC_SCENES_LEVELSELECT_H_
