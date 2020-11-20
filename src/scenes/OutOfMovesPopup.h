#ifndef SRC_SCENES_OUTOFMOVESPOPUP_H_
#define SRC_SCENES_OUTOFMOVESPOPUP_H_

#include "../modules/CommonLayout.h"

using modules::CommonLayout;
using cocos2d::Ref;
using cocos2d::ui::Widget;

namespace scenes {
class OutOfMovesPopup : public CommonLayout {
  private:
    CommonLayout* buttonsLayout;
    CommonLayout* messageLayout;
    int moves;
    void createView();
    void CBBtnCancel(Ref* sender, Widget::TouchEventType type);
    void CBBtnWatch(Ref* sender, Widget::TouchEventType type);
    void swallowTouches();
  public:
    static OutOfMovesPopup* createPopup(int moves);
    CREATE_FUNC(OutOfMovesPopup);
    virtual void onEnter();
};
}  // namespace scenes

#endif  // SRC_SCENES_OUTOFMOVESPOPUP_H_
