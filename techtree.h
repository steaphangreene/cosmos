#include <vector>

class Tech;
class TechTree;

#ifndef TECHTREE_H
#define TECHTREE_H

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
	int res, int ind, int upk, int crw, int loy, int sec, int hap,
	int p1 = -1, int p2 = -1, int p3 = -1, int p4 = -1);
  int type;
  char *name, *names;
  char *desc;
  int rcost, icost;
  int upkeep, crew;
  int loyalty, security, happiness;
  int prereq1;
  int prereq2;
  int prereq3;
  int prereq4;
  vector <int> known;
  };

class TechTree {
public:
  TechTree(int which, int level);
  Tech *GetTech(int w) { return lst[w+num_sciences]; };
  Tech *GetScience(int w) { return lst[w]; };
  int NumSciences() { return num_sciences; };
  int NumTechs() { return num_techs; };

  int Industry(int tnum, int tqty, Planet *plan);

  int Upkeep(int tnum, int tqty, Planet *plan);

  int Minerals(int tnum, int tqty, Planet *plan);
  int Atmosphere(int tnum, int tqty, Planet *plan);
  int Radiation(int tnum, int tqty, Planet *plan);
  int Temperature(int tnum, int tqty, Planet *plan);

  int Crew(int tnum, int tqty, Planet *plan);
  int Loyalty(int tnum, int tqty, Planet *plan);
  int Security(int tnum, int tqty, Planet *plan);
  int Happiness(int tnum, int tqty, Planet *plan);

private:
  void init_tiny(int);
  vector<Tech *> lst;
  int num_sciences;
  int num_techs;
  };

extern TechTree *cur_tree;

#endif
