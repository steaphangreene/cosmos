#include <vector>

using namespace std;

class System;

#ifndef SYSTEM_H
#define SYSTEM_H

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
  vector<Fleet*> fleets;
  vector<int> explored;
  Planet **planets;
  int num_planets;
  int xpos;
  int ypos;
  };

#endif
