#include <cstdio>
#include <cstdlib>
#include <cmath>
#ifndef	M_PI
#define	M_PI	3.14159
#endif

#include "game.h"

//Orbital Period = sqrt(Distance From Star ^ 3)


Planet::Planet(int ord, int sz, int min, int atmos) {
  order = ord;
  size = sz;
  minerals = min;
  atmosphere = atmos;
  int dist = (ord+1)*700;
  period = int(sqrt(double(dist)*double(dist)*double(dist)));
  startpos = rand()&65535;
  };

int Planet::XPos() {
  double ang = double(startpos) + double(cur_game->turn)*double(256) / double(period)*double(256);
  double dist = double(order+1) * double(30);
  double xpos = dist * cos(ang * 2.0 * M_PI / double(65536));
  return int(400+xpos);
  };

int Planet::YPos() {
  double ang = double(startpos) + double(cur_game->turn)*double(256) / double(period)*double(256);
  double dist = double(order+1) * double(30);
  double ypos = dist * sin(ang * 2.0 * M_PI / double(65536));
  return int(384+ypos);
  };
