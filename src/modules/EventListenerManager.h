#ifndef SRC_MODULES_EVENTLISTENERMANAGER_H_
#define SRC_MODULES_EVENTLISTENERMANAGER_H_

#include <string>
#include <vector>

#include "cocos2d.h"

namespace modules {

class EventListenerManager {
 public:
  typedef std::function<void(cocos2d::EventCustom*)> EventHandler;

 protected:
  void initListeners(cocos2d::Node* root);

  virtual void initEventStrings();
  virtual void initEventHandlers();

  std::vector<EventHandler> eventHandlers;
  std::vector<std::string> eventStrs;

 private:
  void initEventListeners();
  void initEventDispatcher();

  std::vector<cocos2d::EventListenerCustom*> eventListeners;
  cocos2d::Node* rootNode;
};

}  // namespace modules
#endif  // SRC_MODULES_EVENTLISTENERMANAGER_H_
