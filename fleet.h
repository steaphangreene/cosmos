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
  Fleet(System *, int, const char *nm = "Some Fleet");
  ~Fleet();
  void TakeTurn();
  int Owner() { return owner; };
  int TimeToLocal(int);
  int TimeToGalactic(int);
  Planet *Target() { return targ; };
  int Distance() { return dist; };
  Planet *Location() { return loc; };
  Planet *Destination() { return dest; };
  int Progress() { return prog; };
  int TripTime() { return trip; };
  void SetCourse(Planet *, int);
  void SetCourse(System *, int);
  void Engage();
  int DetectedBy(int);
  void Detect(int);
  int CanLand();

//private:
  void Arrive();
  int owner;
  string name;
  vector<Ship*> ships;
  Planet *loc;
  Planet *dest;
  int prog, trip;
  Planet *targ;
  int dist;
  vector<int> detected;
  };

#endif
