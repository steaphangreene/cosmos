#include <cstdio>
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
  System(FILE *f);
  virtual ~System();
  virtual void TakeTurn();

  virtual int Owner();
  virtual const char *Name() { return name.c_str(); };
  virtual void SetName(const char *nm) { name = nm; };

  virtual int SType() { return SOBJECT_SYSTEM; }
  virtual int SMove() { return 0; }
  virtual int GMove() { return 0; }

  void FleetLeaves(Fleet *);
  void FleetArrives(Fleet *);

  virtual void Know(int);

  void SaveTo(FILE *);
  void LoadFrom(FILE *);

//protected:
  string name;
  vector<SObject*> objects;
  };

#endif
