#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "math.h"

#include "game.h"

//Orbital Period = sqrt(Distance From Star ^ 3)


Planet::Planet(int ord, int sz, int min, int atmos) {
  order = ord;
  size = sz;
  minerals = min;
  atmosphere = atmos;
  claimed = -1;
  int dist = (ord+1)*700;
  period = int(sqrt(double(dist)*double(dist)*double(dist)));
  startpos = rand()&65535;
  type = 0;
//  type = rand()%4;
  num_satellites = rand()%7;
  satellites = new (Satellite*)[num_satellites];
  for(int ctr=0; ctr<num_satellites; ++ctr) {
    satellites[ctr] = new Satellite(rand()&65535, (rand()&7) <? 1);
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
