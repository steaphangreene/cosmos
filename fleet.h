#include <vector>

using namespace std;

class Fleet;

#ifndef FLEET_H
#define FLEET_H

#include "sobject.h"
#include "ship.h"
#include "planet.h"
#include "system.h"

class Fleet : public SObject {
public:
  Fleet(SObject *, int, const char *nm = "Some Fleet");
  ~Fleet();
  void TakeTurn();
  virtual int Owner() { return owner; };
  virtual int SType() { return SOBJECT_FLEET; }
  int CanLand();

//private:
  int owner;
  vector<Ship*> ships;
  };

#endif
