#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>

#include "game.h"

Game *cur_game = NULL;
TechTree *cur_tree = NULL;

using namespace std;

int max_factions = 64;

void game_init() {
  srand(time(NULL));
  for(int ctr=0; ctr<num_configs; ++ctr) {
    int op;
    for(op=1; config[ctr][op] != NULL; ++op);
    num_options[ctr] = op-1;
    }
  cur_game = new Game;
  }

Game::Game() {
  memset(working_setting, 0, sizeof(working_setting));
  memset(game_setting, 0, sizeof(game_setting));
  memset(setting, 0, sizeof(setting));
  num_players = 0;
  players = NULL;
  num_galaxys = 0;
  galaxys = NULL;
  started = 0;
  }

Game::~Game() {
  Clear();
  }

void Game::TakeTurn() {
  for(int ctr=0; ctr<num_players; ++ctr) players[ctr]->TakeTurn();
  for(int ctr=0; ctr<num_galaxys; ++ctr) galaxys[ctr]->TakeTurn();
  turn++;
  }

void Game::Randomize() {
  for(int ctr=0; ctr<num_configs; ++ctr) {
    if(strcmp("Unknown", config[ctr][1]) == 0)
      working_setting[ctr] = (rand()%(num_options[ctr]-1))+1;
    else
      working_setting[ctr] = (rand()%(num_options[ctr]));
    }
  }

void Game::Reset() {
  for(int ctr=0; ctr<num_configs; ++ctr) {
    working_setting[ctr] = game_setting[ctr];
    }
  }

void Game::ResetToUnknown() {
  memset(working_setting, 0, sizeof(working_setting));
  }

void Game::Finalize() {
  for(int ctr=0; ctr<num_configs; ++ctr) {
    game_setting[ctr] = working_setting[ctr];
    setting[ctr] = working_setting[ctr];
    if(setting[ctr] == 0 && strcmp("Unknown", config[ctr][1]) == 0) {
      setting[ctr] = (rand()%(num_options[ctr]-1))+1;
      }
    }
  }

void Game::Clear() {
  started = 0;
  for(int ctr=0; ctr<num_players; ++ctr) delete players[ctr];
  if(players) delete [] players;
  num_players = 0;
  players = NULL;
  for(int ctr=0; ctr<num_galaxys; ++ctr) delete galaxys[ctr];
  if(galaxys) delete [] galaxys;
  num_galaxys = 0;
  galaxys = NULL;
  }

void Game::Fill() {
  if(started) return;
  Finalize();
  started = 1;
  turn = 0;

  cur_tree = new TechTree(setting[7], setting[8], setting[9]);

  num_players = setting[6];
  players = new (Player*)[num_players];
  for(int ctr=0; ctr<num_players; ++ctr) {
    players[ctr] = new Player;
    }
  players[0]->color = setting[14]+1;
  int numcol = 6;
  int colors[7] = {1, 2, 3, 4, 5, 6, 7};
  colors[setting[14]] = 20;
  for(int ctr=1; ctr<num_players; ++ctr) {
    sort(colors, colors+7);
    int pick = rand()%numcol;
    players[ctr]->color = colors[pick];
    colors[pick] = 20;
    --numcol;
    }

  num_galaxys = setting[10];
  galaxys = new (Galaxy*)[num_galaxys];
  galaxys[0] = new Galaxy(setting[11]*20,
	setting[12], setting[13], cur_tree->Homeworld(),
	num_players, players);
  for(int ctr=1; ctr<num_galaxys; ++ctr) {
    galaxys[ctr] = new Galaxy(setting[11]*20,
	setting[12], setting[13], cur_tree->Homeworld());
    }
  }

const char *config00[] = {	"Ship Combat",
	"Auto Resolution",
//	"2-D",
//	"2-D Tactical",
//	"3-D",
//	"3-D Tactical",
	NULL };

const char *config01[] = {	"Boarding Combat",
	"Auto Resolution",
//	"2-D",
//	"2-D Tactical",
	NULL };

const char *config02[] = {	"Planetary Combat",
	"Auto Resolution",
//	"2-D",
//	"2-D Tactical",
	NULL };

const char *config03[] = {	"Galaxy View",
	"2-D",
//	"3-D",
	NULL };

const char *config04[] = {	"System View",
	"2-D",
//	"3-D",
	NULL };

const char *config05[] = {	"Planet View",
	"2-D",
//	"3-D",
	NULL };

const char *config06[] = {	"Number of Races",
	"Unknown",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
//	"8",
	NULL };

const char *config07[] = {	"Tech Tree",
	"Tiny Test",
//	"MOO2",
//	"MOO2-Dynamic",
//	"Ascend",
//	"Ascend-Dynamic",
//	"Dynamic",
	NULL };

const char *config08[] = {	"Starting Tech Level",
	"Low",
	"Average",
	"High",
	NULL };

const char *config09[] = {	"Starting Devel Level",
	"Low",
	"Average",
	"High",
	NULL };

const char *config10[] = {	"Number of Galaxys",
	"Unknown",
	"1",
//	"2",
//	"Few",
//	"Many",
//	"Unlimited",
	NULL };

const char *config11[] = {	"Galaxy Density",
	"Unknown",
	"Tiny",
	"Small",
	"Medium",
	"Large",
	"Huge",
//	"Massive",
//	"Realistic",
	NULL };

const char *config12[] = {	"Planetary Minerals",
	"Unknown",
	"Low",
	"Average",
	"High",
	NULL };

const char *config13[] = {	"Planetary Atmosphere",
	"Unknown",
	"Thin",
	"Average",
	"Thick",
	NULL };

const char *config14[] = {	"Player Color",
	"@Blue",
	"@Green",
	"@Cyan",
	"@Red",
	"@Purple",
	"@Yellow",
	"@White",
	NULL };

int num_options[num_configs] = {0};
const char **config[num_configs] = {
  config00,
  config01,
  config02,
  config03,
  config04,
  config05,
  config06,
  config07,
  config08,
  config09,
  config10,
  config11,
  config12,
  config13,
  config14
  };
