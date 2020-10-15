#include "Tile.h"

#include "../data/Config.h"

using cocos2d::Vec2;
using data::Config;

namespace views {
const map<string, string> Tile::TileType = {
    {"FLOOR", "_"},    {"WALL", "#"},        {"BOX", "$"},   {"GOAL", "."},
    {"GOAL_BOX", "*"}, {"GOAL_PLAYER", "+"}, {"PLAYER", "@"}};
const map<string, string> Tile::TileFiles = {
    {"_", "./floor"}, {"#", "./wall"},  {"$", "./floor"}, {".", "./floor"},
    {"@", "./floor"}, {"+", "./floor"}, {"*", "./floor"}};
Tile* Tile::createTile(string type) {
  Tile* tile = Tile::create();
  tile->type = type;

  tile->setAnchorPoint(Vec2(0, 0));
  tile->setSpriteFrame(Tile::TileFiles.at(tile->type));
  if (type == TileType.at("GOAL") || type == TileType.at("GOAL_BOX") ||
      type == TileType.at("GOAL_PLAYER")) {
    Sprite* goalSprite = Sprite::createWithSpriteFrameName("./goal");
    goalSprite->setPositionNormalized(Vec2(0.5f, 0.5f));
    goalSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    tile->addChild(goalSprite);
  }
  tile->setScale(Config::DSP_SCALE);
  return tile;
}

}  // namespace views
