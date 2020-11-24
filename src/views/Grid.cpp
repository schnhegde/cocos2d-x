#include "Grid.h"

#include "../data/Config.h"
#include "../data/Events.h"
#include "../modules/EventUtils.h"
#include "../modules/SoundUtil.h"
#include "Entity.h"

using cocos2d::Vec2;
using data::Config;
using data::Events;
using modules::EventUtils;
using modules::SoundUtil;
using std::pair;

namespace views {

Grid* Grid::createGrid(string data, int sizeX, int sizeY) {
  Grid* grid = Grid::create();
  grid->data = data;
  grid->sizeX = sizeX;
  grid->sizeY = sizeY;
  return grid;
}

bool Grid::init() {
  if (!Node::init()) {
    return false;
  }
  setAnchorPoint(Vec2(0.5, 0.5));
  return true;
}

void Grid::onEnter() {
  Node::onEnter();
  createBgTiles();
  createTiles();
  initListeners(this);
  boxMoved = false;
}

void Grid::movePlayer(Vec2 indexInc, bool undo) {
  Vec2 nextIndex = Vec2(player->getIndex().x + indexInc.x,
                        player->getIndex().y + indexInc.y);
  int sIndex = nextIndex.y * sizeX + nextIndex.x;
  string datum(1, data.at(sIndex));
  bool collision = false;
  if (indexInc.x == -1) {
    player->setFlippedX(true);
  } else if (indexInc.x == 1) {
    player->setFlippedX(false);
  }
  if (datum == Tile::TileType.at("WALL")) return;
  if (!(boxes.find(sIndex) == boxes.end())) {
    if (!(moveBox(boxes.at(sIndex), indexInc))) {
      return;
    } else {
      // SoundUtil::getInstance()->playEfxBoxPush();
      collision = true;
      boxMoved = true;
    }
  } else {
    boxMoved = false;
  }

  if (!undo) {
    Move m(indexInc, boxMoved);
    prevMove.push_back(m);
    EventUtils::dispatchEvent(Events::GAME_MOVE_COMPLETED);
  } else {
    EventUtils::dispatchEvent(Events::GAME_UNDO_COMPLETED);
  }
  SoundUtil::getInstance()->playEfxWalk();
  player->setTile(tiles.at(sIndex), nextIndex, "", collision);
}

bool Grid::moveBox(Entity* box, Vec2 inc) {
  int pIndex = box->getIndex().y * sizeX + box->getIndex().x;
  Vec2 nIndex = Vec2(box->getIndex().x + inc.x, box->getIndex().y + inc.y);
  int sIndex = nIndex.y * sizeX + nIndex.x;
  string datum(1, data.at(sIndex));
  bool collision = false;
  if (datum == Tile::TileType.at("WALL")) return false;
  if (!(boxes.find(sIndex) == boxes.end())) {
    return false;
  } else {
    collision = true;
  }

  replaceBoxKey(pIndex, sIndex);
  box->setTile(tiles.at(sIndex), nIndex, datum, collision);

  return true;
}

void Grid::checkPuzzleComplete(EventCustom* event) {
  if (checkPuzzle()) {
    EventUtils::dispatchEvent(Events::GAME_COMPLETED);
  }
}

void Grid::replaceBoxKey(int old, int nVal) {
  auto it = boxes.find(old);
  if (it != boxes.end()) {
    std::swap(boxes[nVal], it->second);
    boxes.erase(it);
  }
}

bool Grid::checkPuzzle() {
  std::map<int, Entity*>::iterator it = boxes.begin();

  while (it != boxes.end()) {
    string tile = string(1, data.at(it->first));
    if (tile != Tile::TileType.at("GOAL") &&
        tile != Tile::TileType.at("GOAL_PLAYER") &&
        tile != Tile::TileType.at("GOAL_BOX")) {
      return false;
    }
    it++;
  }

  return true;
}

void Grid::initEventStrings() {
  EventListenerManager::eventStrs.push_back(Events::GAME_UNDO);
  EventListenerManager::eventStrs.push_back(Events::ANIMATION_COMPLETE);
}

void Grid::initEventHandlers() {
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(Grid::handleUndo, this));
  EventListenerManager::eventHandlers.push_back(
      CC_CALLBACK_1(Grid::checkPuzzleComplete, this));
}

bool Grid::checkIfUndo() { return prevMove.empty(); }

void Grid::handleUndo(EventCustom* event) {
  // undo
  if (prevMove.empty()) return;
  Vec2 pMove = (Vec2)(prevMove.back().moveVec);
  Vec2 nextIndex =
      Vec2(player->getIndex().x + pMove.x, player->getIndex().y + pMove.y);
  int sIndex = nextIndex.y * sizeX + nextIndex.x;

  Vec2 undoMove = Vec2(-(pMove.x), -(pMove.y));
  if (!(boxes.find(sIndex) == boxes.end()) &&
      (static_cast<bool>(prevMove.back().boxMoved))) {
    moveBox(boxes.at(sIndex), undoMove);
  }
  movePlayer(undoMove, true);
  prevMove.pop_back();
}

void Grid::createBgTiles() {
  //    int initialX = - (Config::TILE_SIZE * 11) / 2;
  //    int initialY = (Config::TILE_SIZE * (sizeY + 1)) / 2;
  //     for (int i = 0; i < 11; i++) {
  //      for (int j = 0; j < (sizeY + 1); j++) {
  //        int cIndex = j * 11 + i;
  //        int x = initialX + (i * Config::TILE_SIZE);
  //        int y = initialY - (j * Config::TILE_SIZE);
  //        Sprite* sp = Sprite::create("wall2.png");
  //          sp->setScale(Config::DSP_SCALE);
  //        sp->setAnchorPoint(Vec2(0, 0));
  //        sp->setPosition(Vec2(x, y));
  //        addChild(sp, 0);
  //      }
  //    }
}

void Grid::createTiles() {
  int initialX = -(Config::TILE_SIZE * sizeX) / 2;
  int initialY = (Config::TILE_SIZE * sizeY) / 2;

  for (int i = 0; i < sizeX; i++) {
    for (int j = 0; j < sizeY; j++) {
      int cIndex = j * sizeX + i;
      int x = initialX + (i * Config::TILE_SIZE);
      int y = initialY - (j * Config::TILE_SIZE);
      string datum(1, data.at(cIndex));
      Tile* tile = Tile::createTile(datum);
      tile->setPosition(Vec2(x, y));
      tiles.insert(pair<int, Tile*>(cIndex, tile));
      addChild(tile, 2);

      if (datum == Tile::TileType.at("PLAYER") ||
          datum == Tile::TileType.at("GOAL_PLAYER")) {
        player = Entity::createEntity("player", tile, Vec2(i, j));
        addChild(player, 10);
      } else if (datum == Tile::TileType.at("BOX") ||
                 datum == Tile::TileType.at("GOAL_BOX")) {
        auto box = Entity::createEntity("box", tile, Vec2(i, j), datum);
        boxes.insert(pair<int, Entity*>(cIndex, box));
        addChild(box, 10);
      }
    }
  }
}
}  // namespace views
