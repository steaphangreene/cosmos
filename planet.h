#include <vector>

using namespace std;

#ifndef PLANET_H
#define PLANET_H

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
  int claimed;
  int num_satellites;
  Satellite **satellites;
  vector<int> objs;
  vector<int> oqty;

private:
  int order, size, minerals, atmosphere, temperature, radiation;
  int startpos, period;
  };

#endif
