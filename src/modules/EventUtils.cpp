#include "EventUtils.h"

#include <string>

#include "../data/Events.h"
#include "cocos2d.h"

using data::Events;
using std::string;

namespace modules {

void EventUtils::dispatchEvent(string eventStr) {
  using cocos2d::Director;
  using cocos2d::EventCustom;
  using std::string;

  EventCustom event = EventCustom(eventStr);

  Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

}  // namespace modules
