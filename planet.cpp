#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "game.h"
#include "ship.h"

//Orbital Period = sqrt(Distance From Star ^ 3)

int sattp[5][5] = {
	{ },
	{ 0 },
	{ 1, 1 },
	{ 1, 2, 2 },
	{ 2, 2, 2, 2 },
	};

extern int max_factions;

Planet::Planet(System *s, int ord, int sz, int min, int atmos) {
  system = s;
  order = ord;
  size = sz;
  minerals = min;
  atmosphere = atmos;
  temperature = (10-order)*(10-order)*4-240;
  radiation = (10-order)*(10-order)/2;
  int dist = (ord+1)*700;
  period = int(sqrt(double(dist)*double(dist)*double(dist)));
  startpos = rand()&65535;
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

int Planet::XPos(int turn) {
  double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
  double dist = double(order+1) * double(30);
  double xpos = dist * cos(ang * 2.0 * M_PIl / double(65536));
  return int(384+xpos);
  };

int Planet::YPos(int turn) {
  double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
  double dist = double(order+1) * double(30);
  double ypos = dist * sin(ang * 2.0 * M_PIl / double(65536));
  return int(384+ypos);
  };

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

void Planet::FleetLeaves(Fleet *f) {
  vector<Fleet *>::iterator cur = fleets.begin();
  while(cur < fleets.end()) {
    if(*cur == f) {
      cur = fleets.erase(cur);
      continue;
      }
    ++cur;
    }
  }

void Planet::FleetArrives(Fleet *f) {
  fleets.push_back(f);
  Explore(f->Owner());
  }

int Planet::ExploredBy(int n) {
  return explored[n];
  }

void Planet::Explore(int n) {
  explored[n] = 1;
  }
