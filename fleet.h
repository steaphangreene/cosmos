#include <vector>
#include <string>

using namespace std;

class Fleet;

#ifndef FLEET_H
#define FLEET_H

#include "ship.h"

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

//private:
  int owner;
  int frame;
  string name;
  int xpos, ypos;
  vector<Ship*> ships;
  };

#endif
