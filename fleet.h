#include <vector>
#include <string>

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
  int TimeToLocal(int);
  int TimeToGalactic(int);
  Planet *Target() { return targ; };
  int Distance() { return dist; };
  void SetCourse(Planet *, int);
  void SetCourse(System *, int);
  void Engage();
  int DetectedBy(int);
  void Detect(int);
  int CanLand();

//private:
  int owner;
  string name;
  vector<Ship*> ships;
  Planet *targ;
  int dist;
  vector<int> detected;
  };

#endif
