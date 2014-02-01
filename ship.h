#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Ship;
struct ShipItem;

#ifndef SHIP_H
#define SHIP_H

enum SClass {
  SCLASS_TRANSPORT = 0,
  SCLASS_COLONIZER,
  SCLASS_FIGHTER,
  SCLASS_COLONYSHIP,
  SCLASS_DESTROYER,
  SCLASS_MAX
  };

struct ShipItem {
  int tech_num;
  int size;
  };

class Ship {
public:
  Ship(FILE *);
  Ship(int tec, int own);
  const char *Name() { return name.c_str(); }
  SClass Class() { return sclass; }
  const char *CName();
  int NumItems() { return int(items.size()); }
  ShipItem GetItem(int n) { return items[n]; }
  void AddItem(ShipItem &i) { items.push_back(i); }
  void RemoveItem(int n) { items.erase(items.begin()+n); }
  int Crew() { return crew; }
  int MaxCrew() { return maxcrew; }
  int MinCrew() { return mincrew; }
  int AddCrew(int n)
	{ crew += n; n = crew-maxcrew; crew = min(crew, maxcrew); return max(n, 0); }
  int Owner() { return owner; }
  void TakeTurn();

  int CanLand();
  int SMove();
  int GMove();

  int Offense();
  int Defense();

  void SaveTo(FILE *);
  void LoadFrom(FILE *);

//private:
  int tech;
  string name;
  SClass sclass;
  vector<ShipItem> items;
  int mincrew, maxcrew, crew;
  int owner;
  };

#endif
