#include "./CommonLayout.h"  //NOLINT

using cocos2d::Node;
using cocos2d::Vector;
using cocos2d::ui::Layout;
using cocos2d::ui::Margin;
using cocos2d::ui::Widget;

namespace modules {

bool CommonLayout::init() {
  if (!Layout::init()) {
    return false;
  }

  return true;
}

void CommonLayout::justifyChildren(JUSTIFY justify_type, ALIGN align_type) {
  Vector<Node*> children = this->getChildren();
  ssize_t num_childs = children.size();
  Size layout_size = this->getContentSize();
  Type layout_type = this->getLayoutType();
  int child_skip = justify_type == JUSTIFY::BOOK;
  float n_margin = 0;  // add a fixed offset to objects other than the first one

  Size remaining_space = layout_size - getChildrenSize();

  for (Node* child : children) {
    Widget* child_widget = static_cast<Widget*>(child);

    if (child_widget) {
      Margin margin = Margin();
      Size child_size = child_widget->getContentSize();
      float scale_x = child_widget->getScaleX();
      float scale_y = child_widget->getScaleY();

      if (layout_type == Type::HORIZONTAL) {
        if (child_skip) {
          margin.left = layout_size.width * 0.1;
          n_margin = getJustifyFactor(JUSTIFY::EVENLY, remaining_space.width,
                                      num_childs, children.getIndex(child)) /
                         2 -
                     (margin.left + (child_size.width * scale_x) / 2);
          child_skip = false;
        } else {
          margin.left =
              n_margin + getJustifyFactor(justify_type, remaining_space.width,
                                          num_childs, children.getIndex(child));
        }

        margin.top = (layout_size.height - (child_size.height * scale_y)) *
                     getAlignFactor(align_type);
      } else if (layout_type == Type::VERTICAL) {
        if (child_skip) {
          margin.top = layout_size.height * 0.1;
          n_margin = (getJustifyFactor(JUSTIFY::EVENLY, remaining_space.height,
                                       num_childs, children.getIndex(child)) /
                      2) -
                     (margin.top + (child_size.height * scale_y) / 2);
          child_skip = false;
        } else {
          margin.top =
              n_margin + getJustifyFactor(justify_type, remaining_space.height,
                                          num_childs, children.getIndex(child));
        }

        margin.left = (layout_size.width - (child_size.width * scale_x)) *
                      getAlignFactor(align_type);
      }

      child_widget->getLayoutParameter()->setMargin(margin);
    }
  }
}

float CommonLayout::getJustifyFactor(JUSTIFY justify_type,
                                     float remaining_space, ssize_t num_child,
                                     ssize_t child_index) {
  // must be converted to switch for more than 5 case checks
  if (justify_type == JUSTIFY::EVENLY || justify_type == JUSTIFY::BOOK) {
    return remaining_space / (num_child + 1);
  }

  if (justify_type == JUSTIFY::INSIDE) {
    if (child_index == 0) return 0.0;
    return remaining_space / (num_child - 1);
  }

  if (justify_type == JUSTIFY::OUTSIDE) {
    if (child_index == 0) {
      return remaining_space / 2;
    }
    return 0.0;
  }

  return 0.0;
}

float CommonLayout::getAlignFactor(ALIGN align_type) {
  // must be converted to switch for more than 5 case checks
  if (align_type == ALIGN::CENTER) {
    return 0.5;
  }

  if (align_type == ALIGN::END) {
    return 1.0;
  }

  return 0.0;
}

Size CommonLayout::getChildrenSize() {
  Layout::Type layout_type = this->getLayoutType();
  float w = 0, h = 0;

  Vector<Node*> childs = this->getChildren();
  for (Node* child : childs) {
    Widget* child_widget = static_cast<Widget*>(child);
    if (child_widget) {
      Size child_size = child_widget->getContentSize();
      float scale_x = child_widget->getScaleX();
      float scale_y = child_widget->getScaleY();
      Margin margin = child_widget->getLayoutParameter()->getMargin();

      if (layout_type == Type::HORIZONTAL) {
        w += child_size.width * scale_x;
        float child_height = child_size.height * scale_y;
        h = MAX(h, child_height);
      } else if (layout_type == Type::VERTICAL) {
        float child_width = child_size.width * scale_x;
        w = MAX(w, child_width);
        h += child_size.height * scale_y;
      }
    }
  }

  return Size(w, h);
}

void CommonLayout::setBgColor(Color3B color) {
  this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
  this->setBackGroundColor(color);
}

}  // namespace modules
