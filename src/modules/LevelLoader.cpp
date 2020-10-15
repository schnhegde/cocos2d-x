#include "LevelLoader.h"

namespace modules {
vector<string> LevelLoader::levelData;
vector<int> LevelLoader::countX;
vector<int> LevelLoader::countY;
vector<int> LevelLoader::totalMoves;
vector<string> LevelLoader::levelSolution;

void LevelLoader::loadLevels() {
  string content =
      FileUtils::getInstance()->getStringFromFile("levelData.json");
  Document d;
  d.Parse<0>(content.c_str());
  Value& lstrings = d["level_strings"];
  Value& lsizes = d["level_solutions"];

  for (SizeType i = 0; i < lstrings.Size(); i++) {
    levelData.push_back(lstrings[i].GetString());
  }

  for (SizeType i = 0; i < lsizes.Size(); i++) {
    countX.push_back(lsizes[i]["x"].GetInt());
    countY.push_back(lsizes[i]["y"].GetInt());
    levelSolution.push_back(lsizes[i]["move_string"].GetString());
    totalMoves.push_back(lsizes[i]["total_moves"].GetInt());
  }
}

string LevelLoader::getLevelSolution(int lNo) {
  return levelSolution.at(lNo - 1);
}

string LevelLoader::getLevelData(int lNo) { return levelData.at(lNo - 1); }

int LevelLoader::getLevelSizeX(int lNo) { return countX.at(lNo - 1); }

int LevelLoader::getLevelSizeY(int lNo) { return countY.at(lNo - 1); }

int LevelLoader::getLevelTotalMoves(int lNo) { return totalMoves.at(lNo - 1); }
}  // namespace modules
