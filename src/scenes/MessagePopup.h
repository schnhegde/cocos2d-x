#ifndef SRC_SCENES_MESSAGEPOPUP_H_
#define SRC_SCENES_MESSAGEPOPUP_H_

#include "../modules/CommonLayout.h"

using modules::CommonLayout;
using cocos2d::Ref;
using cocos2d::ui::Widget;

namespace scenes {
class MessagePopup : public CommonLayout {
  private:
    CommonLayout* buttonsLayout;
    CommonLayout* messageLayout;
    std::string message;
    void createView();
    void CBBtnOk(Ref* sender, Widget::TouchEventType type);
    void swallowTouches();
  public:
    static MessagePopup* createPopup(std::string message);
    CREATE_FUNC(MessagePopup);
    virtual void onEnter();
};
}  // namespace scenes

#endif  // SRC_SCENES_MESSAGEPOPUP_H_
