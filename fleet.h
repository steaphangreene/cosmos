#include <vector>
#include <string>

using namespace std;

class Fleet;

#ifndef FLEET_H
#define FLEET_H

#include "ship.h"
#include "planet.h"
#include "system.h"

class Fleet {
public:
  Fleet(int, const char *nm = "Some Fleet");
  ~Fleet();
  void TakeTurn();
  int Owner() { return owner; };
  int XPos() { return xpos; };
  int YPos() { return ypos; };
  int OnFrame() { return frame; }
  void SetPos(int x, int y, int f=0) { xpos = x; ypos = y; frame = f; };
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
  int frame;
  string name;
  int xpos, ypos;
  vector<Ship*> ships;
  Planet *loc;
  Planet *dest;
  int prog, trip;
  Planet *targ;
  int dist;
  vector<int> detected;
  };

#endif
