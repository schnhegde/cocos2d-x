#include "Entity.h"

#include "../data/Config.h"

using cocos2d::FiniteTimeAction;
using cocos2d::MoveTo;
using cocos2d::Sequence;
using cocos2d::Size;
using data::Config;

namespace views {
Entity* Entity::createEntity(string type, Tile* parentTile, Vec2 index,
                             string datum) {
  Entity* entity = Entity::create();
  entity->setAnchorPoint(Vec2(0.5, 0.5));
  entity->setSpriteFrame("./" + type);
  entity->type = type;
  entity->setScale(Config::DSP_SCALE);
  entity->setTile(parentTile, index, datum);
  return entity;
}

void Entity::setTile(Tile* tile, Vec2 index, string datum, bool collision) {
  this->parentTile = tile;
  this->index = index;
  Vec2 pos = parentTile->getPosition();
  float scle;
  if (datum == Tile::TileType.at("GOAL") ||
      datum == Tile::TileType.at("GOAL_PLAYER") ||
      datum == Tile::TileType.at("GOAL_BOX")) {
    setScale(0.8 * Config::DSP_SCALE);
  } else {
    setScale(1 * Config::DSP_SCALE);
  }
  float scl = getScale();
  pos.x += (Config::TILE_SIZE) / 2;
  pos.y += (Config::TILE_SIZE) / 2;
  cocos2d::Vector<FiniteTimeAction*> actions;
  Vec2 oldPos = getPosition();
  Size contentSize = getContentSize();
  if (collision) {
    Vec2 dashPos = Vec2(0, 0);
    if (oldPos.x != pos.x) {
      if (pos.x > oldPos.x) {
        dashPos.x = (contentSize.width) * 0.1;
      } else {
        dashPos.x = -(contentSize.width) * 0.1;
      }
    } else {
      if (pos.y > oldPos.y) {
        dashPos.y = (contentSize.height) * 0.1;
      } else {
        dashPos.y = -(contentSize.height) * 0.1;
      }
    }
    auto action1 = MoveTo::create(0.1, pos + dashPos);
    auto action2 = MoveTo::create(0.1, pos);
    actions.pushBack(action1);
    actions.pushBack(action2);
  } else {
    auto action1 = MoveTo::create(0.1, pos);
    actions.pushBack(action1);
  }
  auto seq = Sequence::create(actions);
  runAction(seq);
}

Vec2 Entity::getIndex() { return index; }
}  // namespace views
