#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "game.h"
#include "position.h"

int MKDIR(const char *fn, int perms) {
  static char buf[1024] = {0};
#ifndef HANDLE_MAX
  if(mkdir(fn, perms)) {
    if(errno != EEXIST) {
      sprintf(buf, "Warning - Can't Create \"%s\"", fn);
      perror(buf);
      return -1;
      }
    }
#else
  if(mkdir(fn)) {
    if(errno != EEXIST) {
      sprintf(buf, "Warning - Can't Create \"%s\"", fn);
      perror(buf);
      return -1;
      }
    }
#endif
  return 0;
  }

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
  local_player = 0;
  started = 0;
  frame = 0;
  }

Game::~Game() {
  Clear();
  }

int Game::Load(const char *fn) {
  static char buf[1024] = {0};
  static char ebuf[1024] = {0};
  if(getenv("HOME"))
    sprintf(buf, "%s/.cosmos/saves/%s", getenv("HOME"), fn);
  else
    sprintf(buf, "saves/%s", fn);

  FILE *svfl = fopen(buf, "r");
  if(!svfl) {
    sprintf(ebuf, "Warning - Can't Load \"%s\"", buf);
    perror(ebuf);
    return -1;
    }
  for(int ctr=0; ctr < num_configs; ++ctr) {
    fscanf(svfl, "%d", &setting[ctr]);
    }
  for(int ctr=0; ctr < num_configs; ++ctr) {
    fscanf(svfl, "%d", &game_setting[ctr]);
    }
  for(int ctr=0; ctr < num_configs; ++ctr) {
    fscanf(svfl, "%d", &working_setting[ctr]);
    }
  fclose(svfl);

  return 0;
  }

int Game::Save(const char *fn) {
  static char buf[1024] = {0};
  static char ebuf[1024] = {0};
  if(getenv("HOME")) {
    sprintf(buf, "%s/.cosmos", getenv("HOME"));
    if(MKDIR(buf, 0755)) return -1;
    sprintf(buf, "%s/.cosmos/saves", getenv("HOME"));
    if(MKDIR(buf, 0755)) return -1;
    sprintf(buf, "%s/.cosmos/saves/%s", getenv("HOME"), fn);
    }
  else {
    if(MKDIR(buf, 0755)) return -1;
    sprintf(buf, "saves/%s", fn);
    }

  FILE *svfl = fopen(buf, "w");
  if(!svfl) {
    sprintf(ebuf, "Warning - Can't Save \"%s\"", buf);
    perror(ebuf);
    return -1;
    }
  for(int ctr=0; ctr < num_configs; ++ctr) {
    fprintf(svfl, "%d", setting[ctr]);
    if(ctr != num_configs-1) fprintf(svfl, " ");
    else fprintf(svfl, "\n");
    }
  for(int ctr=0; ctr < num_configs; ++ctr) {
    fprintf(svfl, "%d", game_setting[ctr]);
    if(ctr != num_configs-1) fprintf(svfl, " ");
    else fprintf(svfl, "\n");
    }
  for(int ctr=0; ctr < num_configs; ++ctr) {
    fprintf(svfl, "%d", working_setting[ctr]);
    if(ctr != num_configs-1) fprintf(svfl, " ");
    else fprintf(svfl, "\n");
    }
  fclose(svfl);

  return 0;
  }

void Game::TakeTurn() {
  EmptyTrash();
  turn++;
  for(int ctr=0; ctr<int(players.size()); ++ctr) players[ctr]->TakeTurn();
  for(int ctr=0; ctr<int(galaxys.size()); ++ctr) galaxys[ctr]->TakeTurn();
  EmptyTrash();
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
  players.clear();
  galaxys.clear();
  //FIXME!!!
  }

void Game::Fill() {
  if(started) return;
  Finalize();
  started = 1;
  turn = 0;

  cur_tree = new TechTree(setting[7], setting[8], setting[9]);

  int num_players = setting[6];
  for(int ctr=0; ctr<num_players; ++ctr) {
    players.push_back(new Player);
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

  int num_galaxys = setting[10];
  galaxys.push_back(new Galaxy(setting[11]*20,
	setting[12], setting[13], players));
  for(int ctr=1; ctr<num_galaxys; ++ctr) {
    galaxys.push_back(new Galaxy(setting[11]*20,
	setting[12], setting[13]));
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

static vector<SObject*> trash;

void Trash(SObject *t) {
  if(t) trash.push_back(t);
  }

void EmptyTrash() {
  CleanPositions(trash);
  for(int ctr=0; ctr<int(trash.size()); ++ctr) {
    trash[ctr]->SetName("Non-Existant Object");
    if(trash[ctr]->SType() == SOBJECT_POSITION)
      RecyclePosition((Position*)trash[ctr]);
    else if(trash[ctr]->SType() == SOBJECT_FLEET) delete(trash[ctr]);
    else delete(trash[ctr]);
    }
  trash.clear();
  }
