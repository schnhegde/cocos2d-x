#ifndef SRC_MODULES_COMMONLAYOUT_H_
#define SRC_MODULES_COMMONLAYOUT_H_

#include <cocos2d.h>
#include <ui/CocosGUI.h>

using cocos2d::Color3B;
using cocos2d::Size;
using cocos2d::ui::Layout;

namespace modules {

class CommonLayout : public Layout {
 public:
  // Align's children in layout direction
  enum class JUSTIFY {
    EVENLY,   // evenly spaceout the children
    INSIDE,   // spaceout between the children
    OUTSIDE,  // divides remaining space at start & end of layout,
    BOOK
  };

  // Align's children in layout opposite direction
  enum class ALIGN { CENTER, START, END };

  virtual bool init();

  // Should set the size of layout before using this method
  void justifyChildren(JUSTIFY, ALIGN align_type = ALIGN::CENTER);

  float getAlignFactor(ALIGN);

  float getJustifyFactor(JUSTIFY, float, ssize_t, ssize_t);

  Size getChildrenSize();

  void setBgColor(Color3B);

  CREATE_FUNC(CommonLayout);
};

}  // namespace modules

#endif  // SRC_MODULES_COMMONLAYOUT_H_
