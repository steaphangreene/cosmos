#include <vector>

class Tech;
class TechTree;

#ifndef TECHTREE_H
#define TECHTREE_H

#include "planet.h"
#include "colony.h"
#include "fleet.h"

using namespace std;

class Planet;

enum {	TECH_NONE=0,
	TECH_STRUCTURE,
	TECH_PROJECT,
	TECH_SHIP,
	TECH_SHIPITEM
	};

int tech_reduce(int, int);
int tech_enhance(int, int);

class Tech {
public:
  Tech(int tp, const char *nm, const char *nms, const char *ds,
	int res, int ind, int upk, int crw, int loy, int sec, int hap, int spc,
	int p1 = -1, int p2 = -1, int p3 = -1, int p4 = -1);
  int type;
  char *name, *names;
  char *desc;
  int rcost, icost;
  int upkeep, crew;
  int loyalty, security, happiness;
  int special;
  int prereq1;
  int prereq2;
  int prereq3;
  int prereq4;
  vector <int> known;
  };

class TechTree {
public:
  TechTree(int which, int techl, int devl);
  Tech *GetTech(int w) { return lst[w+num_sciences]; };
  Tech *GetScience(int w) { return lst[w]; };
  int NumSciences() { return num_sciences; };
  int NumTechs() { return num_techs; };

  int *Homeworld() { return homeworld; }
  int Homeworld(int n) { return homeworld[n]; }

  int Industry(int tnum, int tqty, Colony *col);

  int Upkeep(int tnum, int tqty, Colony *col);

  int Minerals(int tnum, int tqty, Colony *col);
  int Atmosphere(int tnum, int tqty, Colony *col);
  int Radiation(int tnum, int tqty, Colony *col);
  int Temperature(int tnum, int tqty, Colony *col);

  int Crew(int tnum, int tqty, Colony *col);
  int Loyalty(int tnum, int tqty, Colony *col);
  int Security(int tnum, int tqty, Colony *col);
  int Happiness(int tnum, int tqty, Colony *col);

  void SaveTo(FILE *);  void LoadFrom(FILE *);

private:
  int *homeworld;
  void init_tiny(int, int);
  vector<Tech *> lst;
  int num_sciences;
  int num_techs;
  };

extern TechTree *cur_tree;

#endif
