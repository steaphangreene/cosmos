#include <vector>

using namespace std;

class Planet;

#ifndef PLANET_H
#define PLANET_H

#include "fleet.h"
#include "techtree.h"
#include "satellite.h"
#include "system.h"
#include "colony.h"

class Planet {
public:
  Planet(System *s, int ord, int sz, int min, int atmos);
  ~Planet();
  int XPos(int);
  int YPos(int);
  int Type();

  int Radiation();
  int Atmosphere();
  int Temperature();
  int Minerals();

  void TakeTurn();

  int num_satellites;
  Satellite **satellites;
  vector <Colony *> colonies;

  void FleetLeaves(Fleet *);
  void FleetArrives(Fleet *);
  vector<Fleet *> fleets;

  System *Sys() { return system; };

private:
  System *system;
  friend class System;
  int order, size, minerals, atmosphere, temperature, radiation;
  int startpos, period;
  };

#endif
