#include <cstdio>
#include <vector>

using namespace std;

class Fleet;

#ifndef FLEET_H
#define FLEET_H

#include "sobject.h"
#include "planet.h"
#include "system.h"

class Ship;

class Fleet : public SObject {
public:
  Fleet();
  Fleet(SObject *, const char *nm = "Some Fleet");
  Fleet(FILE *);
  virtual ~Fleet();
  virtual void TakeTurn();

  virtual int Owner();
  virtual const char *Name() { return name.c_str(); };
  virtual void SetName(const char *nm) { name = nm; };

  virtual int SType() { return SOBJECT_FLEET; }
  virtual int SMove();             
  virtual int GMove(); 

  int CanLand();
  int NumShips();
  void AddShip(Ship *s);
  Ship *GetShip(int n);
  void RemoveShip(int n);
  void RemoveShips(int s, int e=-1);
  void DestroyShip(int n);
  void DestroyShips(int s, int e=-1);

  int Offense();
  int Defense();
  void Attack(Fleet *);

  void SaveTo(FILE *);
  void LoadFrom(FILE *);

private:
  vector<Ship*> ships;
  string name;
  };

#endif
