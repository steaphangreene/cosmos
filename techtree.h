#include <vector>

using namespace std;

class Planet;

enum {	TECH_NONE=0,
	TECH_PROJECT,
	TECH_BUILDING,
	TECH_SHIP,
	TECH_SHIPITEM
	};

class Tech {
public:
  Tech(int tp, const char *nm, const char *ds, int res, int ind,
	int p1 = -1, int p2 = -1, int p3 = -1, int p4 = -1);
  int type;
  char *name;
  char *desc;
  int rcost, icost;
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

  int Minerals(int tnum, int tqty, Planet *plan);
  int Atmosphere(int tnum, int tqty, Planet *plan);
  int Industry(int tnum, int tqty, int ind, Planet *plan);
  int Radiation(int tnum, int tqty, Planet *plan);
  int Temperature(int tnum, int tqty, Planet *plan);

private:
  void init_tiny(int);
  vector<Tech *> lst;
  int num_sciences;
  int num_techs;
  };

extern TechTree *cur_tree;
