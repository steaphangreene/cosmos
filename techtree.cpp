#include <cmath>
#include <cstdio>

#include "planet.h"
#include "tech_local.h"

extern int max_factions;

int base_tech;

Tech::Tech(int tp, const char *nm, const char *nms, const char *ds,
	int res, int ind, int upk, int crw, int loy, int sec, int hap,
	int p1, int p2, int p3, int p4) {
  type = tp;  name = (char *)nm;  names = (char *)nms; desc = (char *)ds;
  rcost = res;  icost = ind;  upkeep = upk;  crew = crw;
  loyalty = loy;  security = sec;  happiness = hap;
  prereq1 = p1;  prereq2 = p2;  prereq3 = p3;  prereq4 = p4;
  for(int ctr=0; ctr<max_factions; ++ctr) {
    known.push_back(base_tech/rcost);
    }
  }

TechTree::TechTree(int which, int techl) {
  init_tiny(techl);
  }

int TechTree::Minerals(int tnum, int tqty, Planet *plan) {
  return min_tiny(tnum, tqty, plan);
  }

int TechTree::Atmosphere(int tnum, int tqty, Planet *plan) {
  return atm_tiny(tnum, tqty, plan);
  }

int TechTree::Industry(int tnum, int tqty, Planet *plan) {
  return ind_tiny(tnum, tqty, plan);
  }

int TechTree::Upkeep(int tnum, int tqty, Planet *plan) {
  return upk_tiny(tnum, tqty, plan);
  }

int TechTree::Radiation(int tnum, int tqty, Planet *plan) {
  return rad_tiny(tnum, tqty, plan);
  }

int TechTree::Temperature(int tnum, int tqty, Planet *plan) {
  return tmp_tiny(tnum, tqty, plan);
  }

int TechTree::Crew(int tnum, int tqty, Planet *plan) {
  return crw_tiny(tnum, tqty, plan);
  }

int TechTree::Loyalty(int tnum, int tqty, Planet *plan) {
  return loy_tiny(tnum, tqty, plan);
  }

int TechTree::Security(int tnum, int tqty, Planet *plan) {
  return sec_tiny(tnum, tqty, plan);
  }

int TechTree::Happiness(int tnum, int tqty, Planet *plan) {
  return hap_tiny(tnum, tqty, plan);
  }

int tech_reduce(int v, int t) {
  return 0 >? (v - int(double(v)*log10(double(t)))/4);
  }

int tech_enhance(int v, int t) {
  return v;
  }

