#include "galaxy.h"

void game_init();

enum {	TECHTREE_UNKNOWN=0,
	TECHTREE_MOO2,
	TECHTREE_ASCEND,
	TECHTREE_MAX };

enum {	SHIP_UNKNOWN=0,
	SHIP_AUTO,
//	SHIP_2D,
//	SHIP_2D_TAC,
//	SHIP_3D,
//	SHIP_3D_TAC,
	SHIP_MAX };

enum {	BOARD_UNKNOWN=0,
	BOARD_AUTO,
//	BOARD_2D,
//	BOARD_2D_TAC,
	BOARD_MAX };

enum {	PLANET_UNKNOWN=0,
	PLANET_AUTO,
//	PLANET_2D,
//	PLANET_2D_TAC,
	PLANET_MAX };

enum {	GALAXYVIEW_UNKNOWN=0,
	GALAXYVIEW_2D,
//	GALAXYVIEW_3D,
	GALAXYVIEW_MAX };

enum {	SYSTEMVIEW_UNKNOWN=0,
	SYSTEMVIEW_2D,
//	SYSTEMVIEW_3D,
//	SYSTEMVIEW_3D_INT,
	SYSTEMVIEW_MAX };

enum {	PLANETVIEW_UNKNOWN=0,
	PLANETVIEW_2D,
//	PLANETVIEW_3D,
	PLANETVIEW_MAX };

enum {	GALAXYS_UNKNOWN=0,
	GALAXYS_1,
//	GALAXYS_2,
//	GALAXYS_FEW,
//	GALAXYS_MANY,
//	GALAXYS_UNLIMITED,
	GALAXYS_MAX };

enum {	GSIZE_UNKNOWN=0,
	GALAXYS_TINY,
	GALAXYS_SMALL,
	GALAXYS_MEDIUM,
	GALAXYS_LARGE,
	GALAXYS_HUGE,
//	GALAXYS_MASSIVE,
//	GALAXYS_REALISTIC,
	GSIZE_MAX };

enum {	RACES_UNKNOWN=0,
	RACES_1,
	RACES_2,
	RACES_3,
	RACES_4,
	RACES_5,
	RACES_6,
	RACES_7,
	RACES_8,
	RACES_MAX };

enum {	TECH_UNKNOWN=0,
	TECH_LOW,
	TECH_AVERAGE,
	TECH_HIGH,
	TECH_MAX };

enum {	SMINERALS_UNKNOWN=0,
	SMINERALS_LOW,
	SMINERALS_AVERAGE,
	SMINERALS_HIGH,
	SMINERALS_MAX };

enum {	SATMOSPHERE_UNKNOWN=0,
	SATMOSPHERE_LOW,
	SATMOSPHERE_AVERAGE,
	SATMOSPHERE_HIGH,
	SATMOSPHERE_MAX };

class Game {
public:
  Game();
  bool InProgress() { return (started != 0); };
  void Randomize();
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

//private:
  void Finalize();
  int started;
  int s_techtree, s_ship, s_board, s_planet;
  int s_galaxyview, s_systemview, s_planetview;
  int s_galaxys, s_size, s_races, s_techlevel, s_minerals, s_atmosphere;

  int turn, tick;
  int num_galaxys;
  Galaxy **galaxys;
  };

extern Game *cur_game;
