#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "game.h"

//Orbital Period = sqrt(Distance From Star ^ 3)

int sattp[5][5] = {
	{ },
	{ 0 },
	{ 1, 1 },
	{ 1, 2, 2 },
	{ 2, 2, 2, 2 },
	};

Planet::Planet(int ord, int sz, int min, int atmos) {
  order = ord;
  size = sz;
  minerals = min;
  atmosphere = atmos;
  temperature = (10-order)*(10-order)*4-240;
  radiation = (10-order)*(10-order)/2;
  claimed = -1;
  int dist = (ord+1)*700;
  period = int(sqrt(double(dist)*double(dist)*double(dist)));
  startpos = rand()&65535;
  num_satellites = rand()%5;
  satellites = new (Satellite*)[num_satellites];
  for(int ctr=0; ctr<num_satellites; ++ctr) {
    satellites[ctr] = new Satellite(rand()&65535, sattp[num_satellites][ctr]);
    }
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
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    rad += cur_tree->Radiation(objs[ctr], oqty[ctr], this);
    }
  return 0 >? rad;
  }

int Planet::Temperature() {
  int tmp = temperature;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    tmp += cur_tree->Temperature(objs[ctr], oqty[ctr], this);
    }
  return 0 >? tmp;
  }

int Planet::Atmosphere() {
  int atm = atmosphere;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    atm += cur_tree->Atmosphere(objs[ctr], oqty[ctr], this);
    }
  return 0 >? atm;
  }

int Planet::Minerals() {
  int min = minerals;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    min += cur_tree->Minerals(objs[ctr], oqty[ctr], this);
    }
  return 0 >? min;
  }

int Planet::Industry() {
  int ind = 0;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    ind += cur_tree->Industry(objs[ctr], oqty[ctr], ind, this);
    }
  return 0 >? ind;
  }
