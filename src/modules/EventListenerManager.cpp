#include "EventListenerManager.h"

#include "WidgetCBMacros.h"

namespace modules {

void EventListenerManager::initListeners(cocos2d::Node* root) {
  rootNode = root;

  initEventStrings();
  initEventHandlers();
  initEventListeners();
  initEventDispatcher();
}

void EventListenerManager::initEventStrings() {}

void EventListenerManager::initEventHandlers() {}

void EventListenerManager::initEventListeners() {
  using std::string;

  size_t len = eventStrs.size();
  for (int i = 0; i < len; ++i) {
    string event = eventStrs[i];
    EventHandler cb = eventHandlers[i];
    eventListeners.push_back(LISTENER(event, cb));
  }
}

void EventListenerManager::initEventDispatcher() {
  using cocos2d::Director;

  size_t len = eventListeners.size();

  for (int i = 0; i < len; ++i) {
    INIT_DISPATCH(eventListeners[i], rootNode);
  }
}

}  // namespace modules
