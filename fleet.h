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
  Fleet(SObject *, const char *nm = "Some Fleet");
  ~Fleet();
  void TakeTurn();
  virtual int Owner();
  virtual int SType() { return SOBJECT_FLEET; }
  virtual int CanMoveS();             
  virtual int CanMoveG(); 

  int CanLand();
  int NumShips();
  void AddShip(Ship *s);
  Ship *GetShip(int n);
  void RemoveShip(int n);
  void RemoveShips(int s, int e=-1);

private:
  vector<Ship*> ships;
  };

#endif
