#include <vector>

using namespace std;

class Game;
extern Game *cur_game;

#ifndef GAME_H
#define GAME_H

#include "galaxy.h"

void game_init();

const int num_configs = 15;
extern int num_options[];
extern const char **config[];

class Game {
public:
  Game();
  ~Game();
  bool InProgress() { return (started != 0); };
  void Randomize();
  void ResetToUnknown();
  void Reset();
  void Clear();
  void Fill();

  int GetTechTree();
  int GetShipCombat();
  int GetBoardCombat();
  int GetPlanetCombat();

  int GalaxyView();
  int SystemView();
  int PlanetView();

  int GetGalaxys();
  int GetSize();
  int GetRaces();
  int GetTechLevel();
  int GetSystemQuality();

  void TakeTurn();

//private:
  void Finalize();
  int started;
  int setting[num_configs];
  int game_setting[num_configs];
  int working_setting[num_configs];

  int turn, tick, frame;

  vector<Galaxy *>galaxys;
  vector<Player *>players;
  };

#endif
