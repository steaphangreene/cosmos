#include <cstdio>
#include <vector>

using namespace std;

class Colony;

#ifndef COLONY_H
#define COLONY_H

#include "fleet.h"
#include "techtree.h"
#include "satellite.h"
#include "system.h"
#include "planet.h"

class Colony {
public:
  Colony(int o, Planet *p, int initial = 0);
  void Init(int initial);
  ~Colony();

  void LandShip(Ship *);

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

  vector<int> objs;
  vector<int> oqty;
  vector<int> projs;
  vector<int> prog;

  Planet *Plan() { return planet; };

  int Owner() { return owner; };

  void SaveTo(FILE *);
  void LoadFrom(FILE *);

private:
  int owner;
  Planet *planet;
  int population, pop_minor;
  };

#endif
