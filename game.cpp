#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "game.h"

Game *cur_game = NULL;

using namespace std;

void game_init() {
  cur_game = new Game;
  }

Game::Game() {
  started = 0;
  turn = 0;
  Reset();
  }

void Game::Randomize() {
  Reset();
  Finalize();
  }

void Game::Reset() {
  s_techtree = 0;
  s_ship = 0;
  s_board = 0;
  s_planet = 0;
  s_galaxyview = 0;
  s_systemview = 0;
  s_planetview = 0;
  s_galaxys = 0;
  s_size = 0;
  s_races = 0;
  s_techlevel = 0;
  s_minerals = 0;
  s_atmosphere = 0;

  num_galaxys = 0;
  }

void Game::Finalize() {
  srand(time(NULL));
  if(s_techtree == 0) s_techtree = (rand()%(TECHTREE_MAX-1))+1;
  if(s_ship == 0) s_ship = (rand()%(SHIP_MAX-1))+1;
  if(s_board == 0) s_board = (rand()%(BOARD_MAX-1))+1;
  if(s_planet == 0) s_planet = (rand()%(PLANET_MAX-1))+1;
  if(s_galaxyview == 0) s_galaxyview = (rand()%(GALAXYVIEW_MAX-1))+1;
  if(s_systemview == 0) s_systemview = (rand()%(SYSTEMVIEW_MAX-1))+1;
  if(s_planetview == 0) s_planetview = (rand()%(PLANETVIEW_MAX-1))+1;
  if(s_galaxys == 0) s_galaxys = (rand()%(GALAXYS_MAX-1))+1;
  if(s_size == 0) s_size = (rand()%(GSIZE_MAX-1))+1;
  if(s_races == 0) s_races = (rand()%(RACES_MAX-1))+1;
  if(s_techlevel == 0) s_techlevel = (rand()%(TECH_MAX-1))+1;
  if(s_minerals == 0) s_minerals = (rand()%(SMINERALS_MAX-1))+1;
  if(s_atmosphere == 0) s_atmosphere = (rand()%(SATMOSPHERE_MAX-1))+1;
  }

void Game::Clear() {
  started = 0;
  num_galaxys = 0;
  }

void Game::Fill() {
  if(started) return;
  Finalize();
  started = 1;

/*
  printf("Tech Tree = %d\n", s_techtree);
  printf("Ship Combat System = %d\n", s_ship);
  printf("Boarding Combat System = %d\n", s_board);
  printf("Planetary Combat System = %d\n", s_planet);
  printf("Galaxy View = %d\n", s_galaxys);
  printf("System View = %d\n", s_systemview);
  printf("Planetary View = %d\n", s_planetview);
  printf("Numer of Galaxys = %d\n", s_galaxys);
  printf("Galaxy Size = %d\n", s_size);
  printf("Number of Races = %d\n", s_races);
  printf("Tech Level = %d\n", s_techlevel);
  printf("Planet Minerals = %d\n", s_minerals);
  printf("Planet Atmosphere = %d\n", s_atmosphere);
*/

  num_galaxys = s_galaxys;
  galaxys = new (Galaxy*)[num_galaxys];
  for(int ctr=0; ctr<num_galaxys; ++ctr) {
    galaxys[ctr] = new Galaxy(s_size*20, s_minerals, s_atmosphere);
    }
  }
