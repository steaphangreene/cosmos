#include <cmath>
#include <cstdio>

#include "planet.h"
#include "tech_local.h"

extern int max_factions;

int base_tech;

Tech::Tech(int tp, const char *nm, const char *nms, const char *ds,
	int res, int ind, int upk, int crw, int loy, int sec, int hap, int spc,
	int p1, int p2, int p3, int p4) {
  type = tp;  name = (char *)nm;  names = (char *)nms; desc = (char *)ds;
  rcost = res;  icost = ind;  upkeep = upk;  crew = crw;
  loyalty = loy;  security = sec;  happiness = hap;  special = spc;
  prereq1 = p1;  prereq2 = p2;  prereq3 = p3;  prereq4 = p4;
  for(int ctr=0; ctr<max_factions; ++ctr) {
    known.push_back(base_tech/rcost);
    }
  }

TechTree::TechTree(int which, int techl, int devl) {
  init_tiny(techl, devl);
  for(int ctr=0; ctr<int(lst.size()); ++ctr) {
    if(lst[ctr]->known[0] == 0) homeworld[ctr] = 0;
    }
  }

int TechTree::Minerals(int tnum, int tqty, Colony *col) {
  return min_tiny(tnum, tqty, col);
  }

int TechTree::Atmosphere(int tnum, int tqty, Colony *col) {
  return atm_tiny(tnum, tqty, col);
  }

int TechTree::Industry(int tnum, int tqty, Colony *col) {
  return ind_tiny(tnum, tqty, col);
  }

int TechTree::Upkeep(int tnum, int tqty, Colony *col) {
  return upk_tiny(tnum, tqty, col);
  }

int TechTree::Radiation(int tnum, int tqty, Colony *col) {
  return rad_tiny(tnum, tqty, col);
  }

int TechTree::Temperature(int tnum, int tqty, Colony *col) {
  return tmp_tiny(tnum, tqty, col);
  }

int TechTree::Crew(int tnum, int tqty, Colony *col) {
  return crw_tiny(tnum, tqty, col);
  }

int TechTree::Loyalty(int tnum, int tqty, Colony *col) {
  return loy_tiny(tnum, tqty, col);
  }

int TechTree::Security(int tnum, int tqty, Colony *col) {
  return sec_tiny(tnum, tqty, col);
  }

int TechTree::Happiness(int tnum, int tqty, Colony *col) {
  return hap_tiny(tnum, tqty, col);
  }

int tech_reduce(int v, int t) {
  if(v <= 1) return 0;
  return 0 >? (v - int(double(v)*log10(double(t)))/4);
  }

int tech_enhance(int v, int t) {
  return v;
  }

void TechTree::SaveTo(FILE *f) {
  }

void TechTree::LoadFrom(FILE *f) {
  }
