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
  type = rand()%5;
  };

int Planet::XPos(int turn) {
  double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
  double dist = double(order+1) * double(30);
  double xpos = dist * cos(ang * 2.0 * M_PI / double(65536));
  return int(400+xpos);
  };

int Planet::YPos(int turn) {
  double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
  double dist = double(order+1) * double(30);
  double ypos = dist * sin(ang * 2.0 * M_PI / double(65536));
  return int(384+ypos);
  };
