#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "game.h"
#include "ship.h"

int sattp[5][5] = {
	{ },
	{ 0 },
	{ 1, 1 },
	{ 1, 2, 2 },
	{ 2, 2, 2, 2 },
	};

extern int max_factions;

Planet::Planet(System *s, int ord, int sz, int min, int atmos)
  : SObject(s, (ord+1)*600){
  size = sz;
  minerals = min;
  atmosphere = atmos;
  temperature = (10-ord)*(10-ord)*4-240;
  radiation = (10-ord)*(10-ord)/2;
  num_satellites = (rand()%4)+1;
  satellites = new (Satellite*)[num_satellites];
  for(int ctr=0; ctr<num_satellites; ++ctr) {
    satellites[ctr] = new Satellite(rand()&65535, sattp[num_satellites][ctr]);
    }
  explored.resize(max_factions, 0);
  };

Planet::~Planet() {
  for(int ctr=0; ctr < num_satellites; ++ctr) {
    delete satellites[ctr];
    }
  delete [] satellites;
  satellites = NULL;
  }

int Planet::Type() {
  return 0;
  }

int Planet::Radiation() {
  int rad = radiation-Atmosphere();
  for(int col=0; col<int(colonies.size()); ++col) {
    for(int ctr=0; ctr<int(colonies[col]->objs.size()); ++ctr) {
      rad += cur_tree->Radiation(colonies[col]->objs[ctr],
		colonies[col]->oqty[ctr], colonies[col]);
      }
    }
  return 0 >? rad;
  }

int Planet::Temperature() {
  int tmp = temperature;
  for(int col=0; col<int(colonies.size()); ++col) {
    for(int ctr=0; ctr<int(colonies[col]->objs.size()); ++ctr) {
      tmp += cur_tree->Temperature(colonies[col]->objs[ctr],
		colonies[col]->oqty[ctr], colonies[col]);
      }
    }
  return 0 >? tmp;
  }

int Planet::Atmosphere() {
  int atm = atmosphere;
  for(int col=0; col<int(colonies.size()); ++col) {
    for(int ctr=0; ctr<int(colonies[col]->objs.size()); ++ctr) {
      atm += cur_tree->Atmosphere(colonies[col]->objs[ctr],
		colonies[col]->oqty[ctr], colonies[col]);
      }
    }
  return 0 >? atm;
  }

int Planet::Minerals() {
  int min = minerals;
  for(int col=0; col<int(colonies.size()); ++col) {
    for(int ctr=0; ctr<int(colonies[col]->objs.size()); ++ctr) {
      min += cur_tree->Minerals(colonies[col]->objs[ctr],
		colonies[col]->oqty[ctr], colonies[col]);
      }
    }
  return 0 >? min;
  }

void Planet::TakeTurn() {
  for(int ctr=0; ctr<int(colonies.size()); ++ctr) colonies[ctr]->TakeTurn();
  for(int ctr=0; ctr<num_satellites; ++ctr) satellites[ctr]->TakeTurn();
  }

int Planet::ExploredBy(int n) {
  return explored[n];
  }

void Planet::Explore(int n) {
  explored[n] = 1;
  }
