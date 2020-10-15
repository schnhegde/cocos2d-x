#ifndef SRC_MODULES_LEVELLOADER_H_
#define SRC_MODULES_LEVELLOADER_H_

#include "../../cocos2d/cocos/editor-support/spine/Json.h"
#include "cocos2d.h"
#include "external/json/document.h"

using cocos2d::FileUtils;
using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;
using std::string;
using std::vector;

namespace modules {
class LevelLoader {
 private:
  static vector<string> levelData;
  static vector<int> countX;
  static vector<int> countY;
  static vector<int> totalMoves;
  static vector<string> levelSolution;

 public:
  static void loadLevels();
  static string getLevelData(int lNo);
  static int getLevelSizeX(int lno);
  static int getLevelSizeY(int lno);
  static int getLevelTotalMoves(int lno);
  static string getLevelSolution(int lno);
};
}  // namespace modules

#endif  // SRC_MODULES_LEVELLOADER_H_
