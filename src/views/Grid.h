#ifndef SRC_VIEWS_GRID_H_
#define SRC_VIEWS_GRID_H_

#include <map>

#include "../modules/EventListenerManager.h"
#include "Entity.h"
#include "Tile.h"
#include "cocos2d.h"

using cocos2d::EventCustom;
using cocos2d::Node;
using modules::EventListenerManager;
using std::map;
using std::string;
using std::vector;

namespace views {
class Move {
 public:
  Move(Vec2 move, bool box) {
    moveVec = move;
    boxMoved = box;
  }
  Vec2 moveVec;
  bool boxMoved;
};
class Grid : public Node, public EventListenerManager {
 public:
  static Grid* createGrid(string data, int sizeX, int sizeY);
  CREATE_FUNC(Grid);

  virtual bool init();
  virtual void onEnter();
  virtual void initEventStrings();
  virtual void initEventHandlers();
  void movePlayer(Vec2 inc, bool undo = false);
  bool moveBox(Entity* box, Vec2 inc);
  bool checkIfUndo();

 private:
  string data;
  int sizeX;
  int sizeY;
  map<int, Tile*> tiles;
  Entity* player;
  map<int, Entity*> boxes;
  std::vector<Move> prevMove;
  bool boxMoved;
  void createBgTiles();
  void handleUndo(EventCustom* event);
  void checkPuzzleComplete(EventCustom* event);
  void showNoMovesPopup();
  bool checkMoves();
  bool checkPuzzle();
  void createTiles();
  void replaceBoxKey(int old, int nVal);
};
}  // namespace views

#endif  // SRC_VIEWS_GRID_H_
