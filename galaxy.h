#include <vector>

using namespace std;

#ifndef GALAXY_H
#define GALAXY_H

#include "player.h"
#include "system.h"

class Galaxy {
public:
  Galaxy(int numsys, int minl, int atmosl,
	const vector<Player *> &pl = vector<Player *>());

  void TakeTurn();

//private:
  System **systems;
  int num_systems;
  };

#endif
