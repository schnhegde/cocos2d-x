#ifndef SRC_VIEWS_ENTITY_H_
#define SRC_VIEWS_ENTITY_H_

#include <map>

#include "Tile.h"
#include "cocos2d.h"

using cocos2d::Sprite;
using cocos2d::Vec2;
using std::map;
using std::string;

namespace views {
class Entity : public Sprite {
 public:
  static Entity* createEntity(string type, Tile* tile, Vec2 index,
                              string datum = "");
  CREATE_FUNC(Entity);
  void setTile(Tile* tile, Vec2 index, string datum = "",
               bool collision = false);
  Vec2 getIndex();

 private:
  string type;
  Vec2 index;
  Tile* parentTile;
};
}  // namespace views

#endif  // SRC_VIEWS_ENTITY_H_
