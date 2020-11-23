#ifndef SRC_SCENES_WATCHVIDEOPOPUP_H_
#define SRC_SCENES_WATCHVIDEOPOPUP_H_

#include "../modules/CommonLayout.h"

using modules::CommonLayout;
using cocos2d::Ref;
using cocos2d::ui::Widget;

namespace scenes {
class WatchVideoPopup : public CommonLayout {
  private:
    CommonLayout* buttonsLayout;
    CommonLayout* messageLayout;
    std::string reward;
    int rewardAmount;
    void createView();
    void CBBtnCancel(Ref* sender, Widget::TouchEventType type);
    void CBBtnWatch(Ref* sender, Widget::TouchEventType type);
    void swallowTouches();
  public:
    static WatchVideoPopup* createPopup(std::string reward, int rewardAmount = 1);
    CREATE_FUNC(WatchVideoPopup);
    virtual void onEnter();
};
}  // namespace scenes

#endif  // SRC_SCENES_WATCHVIDEOPOPUP_H_
