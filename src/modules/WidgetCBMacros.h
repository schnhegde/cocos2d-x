/**************************
************
*
* Copyright (c) 2015 Playzio Pvt. Ltd.
*
* http://www.playzio.net
*
**************************************/

#ifndef SRC_MODULES_WIDGETCBMACROS_H_
#define SRC_MODULES_WIDGETCBMACROS_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CocosStudioExport.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"
#include "ui/CocosGUI.h"

typedef cocos2d::ui::Widget::ccWidgetTouchCallback ccWTCB;
typedef cocos2d::ui::Widget::ccWidgetClickCallback ccWCCB;
typedef cocos2d::ui::Widget::ccWidgetEventCallback ccWECB;
typedef cocos2d::ui::Widget::TouchEventType ccTET;
typedef const std::string CSSTR;

/** @def DECLARE_UI_CALLBACK_METHODS(__CLASSNAME__)
 * declare the custome class which will implement ui callback.
 *
 * @param __CLASSNAME__  name of the class.
 */
#define DECLARE_UI_CALLBACK_METHODS(__CLASSNAME__)           \
 public:                                                     \
  CREATE_FUNC(__CLASSNAME__);                                \
                                                             \
  __CLASSNAME__(){};                                         \
                                                             \
  virtual ccWTCB onLocateTouchCallback(CSSTR& callBackName); \
  virtual ccWCCB onLocateClickCallback(CSSTR& callBackName); \
  virtual ccWECB onLocateEventCallback(CSSTR& callBackName); \
                                                             \
  void onTouch(cocos2d::Ref* sender, ccTET type);            \
  void onClick(cocos2d::Ref* sender);                        \
  void onEvent(cocos2d::Ref* sender, int eventType);         \
                                                             \
 private:                                                    \
  std::vector<std::string> _touchTypes;                      \
  std::string _click;                                        \
  std::vector<std::string> _eventTypes;                      \
  cocos2d::Node* rootNode;

/** @def DEFINE_UI_CALLBACKS(__CLASSNAME__)
 * Override the callbacks which are derived from the
 * cocostudio::WidgetCallBackHandlerProtocol class.
 *
 * @param __CLASSNAME__  classname to add these functions to.
 */
#define DEFINE_UI_CALLBACKS(__CLASSNAME__)                           \
  ccWTCB __CLASSNAME__::onLocateTouchCallback(CSSTR& callBackName) { \
    if (callBackName == "onTouch") {                                 \
      return CC_CALLBACK_2(__CLASSNAME__::onTouch, this);            \
    }                                                                \
    return nullptr;                                                  \
  }                                                                  \
  ccWCCB __CLASSNAME__::onLocateClickCallback(CSSTR& callBackName) { \
    if (callBackName == "onClick") {                                 \
      return CC_CALLBACK_1(__CLASSNAME__::onClick, this);            \
    }                                                                \
    return nullptr;                                                  \
  }                                                                  \
  ccWECB __CLASSNAME__::onLocateEventCallback(CSSTR& callBackName) { \
    if (callBackName == "onEvent") {                                 \
      return CC_CALLBACK_2(__CLASSNAME__::onEvent, this);            \
    }                                                                \
    return nullptr;                                                  \
  }

/** @def DEFINE_SCENE_CREATE(__CLASSNAME__)
 * Create the createScene implementation.
 *
 * @param __CLASSNAME__  scene class name
 */
#define DEFINE_SCENE_CREATE(__CLASSNAME__)                          \
  std::string className = #__CLASSNAME__;                           \
  auto scene = Scene::create();                                     \
  CSLoader* instance = CSLoader::getInstance();                     \
  instance->registReaderObject(                                     \
      className + "Reader",                                         \
      (ObjectFactory::Instance)__CLASSNAME__##Reader::getInstance); \
  auto rootNode = CSLoader::createNode(className + ".csb");         \
  scene->addChild(rootNode);                                        \
  return scene;

/** @def DEFINE_SCENE_CREATE(__CLASSNAME__)
 * Create the createScene implementation.
 *
 * @param __CLASSNAME__  scene class name
 */
#define DEFINE_MULTIPLAYER_SCENE_CREATE(__CLASSNAME__)              \
  std::string className = #__CLASSNAME__;                           \
  auto scene = Scene::create();                                     \
  CSLoader* instance = CSLoader::getInstance();                     \
  instance->registReaderObject(                                     \
      className + "Reader",                                         \
      (ObjectFactory::Instance)__CLASSNAME__##Reader::getInstance); \
  className = className + "Multiplayer";                            \
  auto rootNode = CSLoader::createNode(className + ".csb");         \
  scene->addChild(rootNode);                                        \
  return scene;

/** @def DEFINE_SCENE_THEME_CREATE(__CLASSNAME__, __THEMENAME__)
 * Create the createScene implementation.
 * Load specific theme file from the disk
 *
 * @param __CLASSNAME__ scene class name
 * @param __THEMENAME__ string complete theme file name with extention
 */
#define DEFINE_SCENE_THEME_CREATE(__CLASSNAME__, __THEMENAME__)     \
  std::string className_ = #__CLASSNAME__;                          \
  auto scene_ = Scene::create();                                    \
  CSLoader* instance = CSLoader::getInstance();                     \
  instance->registReaderObject(                                     \
      className_ + "Reader",                                        \
      (ObjectFactory::Instance)__CLASSNAME__##Reader::getInstance); \
  auto rootNode_ = CSLoader::createNode(__THEMENAME__);             \
  scene_->addChild(rootNode_);                                      \
  return scene_;

/** @def DEFINE_TOUCH_CALLBACK_STUB(__CLASSNAME__)
 * define an empty stub for onTouch() handler
 *
 * @param __CLASSNAME__  classname to add these functions to.
 */
#define DEFINE_TOUCH_CALLBACK_STUB(__CLASSNAME__)                 \
  void __CLASSNAME__::onTouch(cocos2d::Ref* sender, ccTET type) { \
    CCLOG("onTouch");                                             \
  }

/** @def DEFINE_CLICK_CALLBACK_STUB(__CLASSNAME__)
 * define an empty stub for onClick() handler
 *
 * @param __CLASSNAME__  classname to add these functions to.
 */
#define DEFINE_CLICK_CALLBACK_STUB(__CLASSNAME__) \
  void __CLASSNAME__::onClick(cocos2d::Ref* sender) { CCLOG("onClick"); }

/** @def DEFINE_EVENT_CALLBACK_STUB(__CLASSNAME__)
 * define an empty stub for onEvent() handler
 *
 * @param __CLASSNAME__  classname to add these functions to.
 */
#define DEFINE_EVENT_CALLBACK_STUB(__CLASSNAME__)                    \
  void __CLASSNAME__::onEvent(cocos2d::Ref* sender, int eventType) { \
    CCLOG("onEvent");                                                \
  }

/** @def DECLARE_UI_READER_CLASS(__CLASSNAME__)
 * declare the custom class which will implement ui reader.
 *
 * @param __CLASSNAME__  name of the ui class.
 */
#define DECLARE_UI_READER_CLASS(__CLASSNAME__)                  \
  class __CLASSNAME__##Reader : public cocostudio::NodeReader { \
   public:                                                      \
    __CLASSNAME__##Reader() {}                                  \
    ~__CLASSNAME__##Reader() {}                                 \
                                                                \
    static __CLASSNAME__##Reader* getInstance();                \
    static void purge();                                        \
    cocos2d::Node* createNodeWithFlatBuffers(                   \
        const flatbuffers::Table* nodeOptions);                 \
  };

/** @def DECLARE_KEYBOARD_HANDLER
 * declare the keyboard handler functions
 *
 */
#define DECLARE_KEYBOARD_HANDLER                              \
  void addBackButtonListener();                               \
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, \
                     cocos2d::Event* event);                  \
  void handleBackButton();

/** @def DEFINE_KEYBOARD_HANDLER_LISTENER(__CLASSNAME__)
 * define the keyboard handler functions
 *
 */
#define DEFINE_KEYBOARD_HANDLER_LISTENER(__CLASSNAME__)                       \
  void __CLASSNAME__::addBackButtonListener() {                               \
    auto listener = EventListenerKeyboard::create();                          \
                                                                              \
    listener->onKeyReleased =                                                 \
        CC_CALLBACK_2(__CLASSNAME__::onKeyReleased, this);                    \
                                                                              \
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); \
  }

/** @def DEFINE_KEYBOARD_HANDLER_CALLBACK(__CLASSNAME__)
 * define the keyboard callback
 *
 */
#define DEFINE_KEYBOARD_HANDLER_CALLBACK(__CLASSNAME__)             \
  void __CLASSNAME__::onKeyReleased(EventKeyboard::KeyCode keyCode, \
                                    Event* event) {                 \
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {            \
      SoundUtils::getInstance()->playEfxBtnTouched();               \
                                                                    \
      handleBackButton();                                           \
    }                                                               \
  }

/** @def DEFINE_KEYBOARD_HANDLER(__CLASSNAME__)
 * define the keyboard callback and listener
 *
 */
#define DEFINE_KEYBOARD_HANDLER(__CLASSNAME__)    \
  DEFINE_KEYBOARD_HANDLER_LISTENER(__CLASSNAME__) \
  DEFINE_KEYBOARD_HANDLER_CALLBACK(__CLASSNAME__)

/**
 * @def add event listener to dispatcher
 */
#define INIT_DISPATCH(__LISTENER__, __ROOT__) \
  Director::getInstance()                     \
      ->getEventDispatcher()                  \
      ->addEventListenerWithSceneGraphPriority(__LISTENER__, __ROOT__);

/**
 * @def Add button callback mapping declarations
 *
 * @param __CLASSNAME__ class name for which function
 * pointer needs to be declared
 */
#define DECLARE_BTN_CB_MAPPING(__CLASSNAME__)          \
 public:                                               \
  typedef void (__CLASSNAME__::*BtnCallback)();        \
                                                       \
  typedef std::map<std::string, BtnCallback> CBBtnMap; \
                                                       \
 private:                                              \
  void initBtnGUIIIDs();                               \
                                                       \
  void initBtnCallbacks();                             \
                                                       \
  void initBtnCallbackMapping();                       \
                                                       \
  std::vector<std::string> btnGUIIDs;                  \
                                                       \
  std::vector<BtnCallback> btnCallbacks;               \
                                                       \
  CBBtnMap cbBtnMap;

/**
 * @def Add button callback mapping implementation
 *
 * @param __CLASSNAME__ class name for which functions
 * needs to be implemented
 */
#define DEFINE_BTN_CB_MAPPING(__CLASSNAME__)     \
  void __CLASSNAME__::initBtnCallbackMapping() { \
    using std::string;                           \
    initBtnGUIIIDs();                            \
    initBtnCallbacks();                          \
                                                 \
    size_t cbLen = btnGUIIDs.size();             \
                                                 \
    for (size_t i = 0; i < cbLen; ++i) {         \
      string key = btnGUIIDs[i];                 \
      BtnCallback callback = btnCallbacks[i];    \
      cbBtnMap.insert(make_pair(key, callback)); \
    }                                            \
  }

/**
 * @def Dispatch call to appropriate function
 *
 * @param __SENDER__ cocos2d::Ref* sender
 */
#define DISPATCH_BTN_CALL(__SENDER__)          \
  using std::string;                           \
  using cocos2d::ui::Button;                   \
  Button* btn = dynamic_cast<Button*>(sender); \
  string btnName = btn->getName();             \
  BtnCallback callback = cbBtnMap[btnName];    \
  (this->*callback)();

/**
 * @def create a custom event listener
 */
#define LISTENER(__EVENT__, __HANDLER__) \
  cocos2d::EventListenerCustom::create(__EVENT__, __HANDLER__)

/**
 * @def Macro for event dispatcher
 */
#define DISPATCHER cocos2d::Director::getInstance()->getEventDispatcher()

/**
 * @def Macro for adding event listener
 */
#define ADD_LISTENER addEventListenerWithSceneGraphPriority

#endif  // SRC_MODULES_WIDGETCBMACROS_H_
