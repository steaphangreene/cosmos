#include <vector>

using namespace std;

class Planet;

#ifndef PLANET_H
#define PLANET_H

#include "sobject.h"
#include "fleet.h"
#include "techtree.h"
#include "satellite.h"
#include "system.h"
#include "colony.h"

class Planet : public SObject {
public:
  Planet(System *s, int ord, int sz, int min, int atmos);
  ~Planet();
  int Type();
  virtual int Owner();
  virtual int SType() { return SOBJECT_PLANET; }
  virtual int SMove();
  virtual int GMove();

  int Radiation();
  int Atmosphere();
  int Temperature();
  int Minerals();

  virtual void TakeTurn();

  int num_satellites;
  Satellite **satellites;
  vector <Colony *> colonies;

protected:
  friend class System;
  int order, size, minerals, atmosphere, temperature, radiation;
  };

#endif
