#ifndef SRC_VIEWS_TILE_H_
#define SRC_VIEWS_TILE_H_

#include <map>

#include "cocos2d.h"

using cocos2d::Sprite;
using std::map;
using std::string;

namespace views {
class Tile : public Sprite {
 public:
  static const map<string, string> TileType;
  static const map<string, string> TileFiles;
  static Tile* createTile(string type);
  CREATE_FUNC(Tile);

 private:
  string type;
};
}  // namespace views

#endif  // SRC_VIEWS_TILE_H_
