#ifndef SRC_MODULES_EVENTUTILS_H_
#define SRC_MODULES_EVENTUTILS_H_

#include <string>

using std::string;

namespace modules {

class EventUtils {
 public:
  static void dispatchEvent(string eventStr);
};

}  // namespace modules
#endif  // SRC_MODULES_EVENTUTILS_H_
