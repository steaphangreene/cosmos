#include <vector>

using namespace std;

#ifndef PLANET_H
#define PLANET_H

#include "fleet.h"
#include "techtree.h"
#include "satellite.h"

class Planet {
public:
  Planet(int ord, int sz, int min, int atmos);
  ~Planet();
  int XPos(int);
  int YPos(int);
  int Type();

  int Radiation();
  int Atmosphere();
  int Temperature();
  int Minerals();

  int Industry();
  int SpareIndustry();

  int Growth();
  int GrowthM();
  int Population();
  int PopulationM();
  int FreePop();
  int FreePopM();

  int Happiness();
  int Loyalty();
  int Security();

  void TakeTurn();

  int num_satellites;
  Satellite **satellites;
  vector<int> objs;
  vector<int> oqty;
  vector<int> projs;
  vector<int> prog;
  vector<Fleet*> fleets;

  int claimed;
private:
  int population, pop_minor;
  friend class System;
  int order, size, minerals, atmosphere, temperature, radiation;
  int startpos, period;
  };

#endif
