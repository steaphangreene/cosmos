#include <vector>

using namespace std;

class System;

#ifndef SYSTEM_H
#define SYSTEM_H

#include "sobject.h"
#include "planet.h"
#include "fleet.h"

class System : public SObject {
public:
  System(int xp, int yp, int nump, int minl, int atmosl, int pl=-1);

  virtual ~System();

  virtual int Owner();
  virtual void TakeTurn();

  virtual int SType() { return SOBJECT_SYSTEM; }
  virtual int SMove() { return 0; }
  virtual int GMove() { return 0; }

  void FleetLeaves(Fleet *);
  void FleetArrives(Fleet *);

  virtual void Know(int);

//private:
  vector<SObject*> objects;
  };

#endif
