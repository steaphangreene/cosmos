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
  virtual ~Planet();
  virtual int SType() { return SOBJECT_PLANET; }

  virtual int Owner();
  virtual const char *Name() { return name.c_str(); };
  virtual void SetName(const char *nm) { name = nm; };

  virtual int SMove();
  virtual int GMove();

  int Type();

  int Radiation();
  int Atmosphere();
  int Temperature();
  int Minerals();

  virtual void TakeTurn();

  int num_satellites;
  Satellite **satellites;
  vector <Colony *> colonies;

protected:
  string name;
  friend class System;
  int order, size, minerals, atmosphere, temperature, radiation;
  };

#endif
