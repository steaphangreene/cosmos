#include <cstdio>
#include <vector>

using namespace std;

#ifndef GALAXY_H
#define GALAXY_H

#include "player.h"
#include "system.h"

class Galaxy {
public:
  Galaxy(FILE *);
  Galaxy(int numsys, int minl, int atmosl,
	const vector<Player *> &pl = vector<Player *>());
  ~Galaxy();

  void TakeTurn();
  void SaveTo(FILE *);
  void LoadFrom(FILE *);

//private:
  vector <System*> systems;
  };

#endif
