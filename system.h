#include <vector>

using namespace std;

class System;

#ifndef SYSTEM_H
#define SYSTEM_H

#include "sobject.h"
#include "planet.h"
#include "fleet.h"

class System {
public:
  System(int xp, int yp, int nump, int minl, int atmosl, int pl=-1);
  ~System();

  void TakeTurn();
  void FleetLeaves(Fleet *);
  void FleetArrives(Fleet *);
  int ExploredBy(int);
  void Explore(int);
  int Owner();

//private:
  vector<SObject*> objects;
  vector<SObject*> fleets;
  vector<int> explored;
  int xpos;
  int ypos;
  };

#endif
