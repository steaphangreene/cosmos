#include <cstdio>

#ifndef TECHTREE_H
#define TECHTREE_H

#include "planet.h"

static int base_tech;
static int max_factions;

void init_tiny(int);
int min_tiny(int tnum, int tqty, Planet *plan);
int atm_tiny(int tnum, int tqty, Planet *plan);
int ind_tiny(int tnum, int tqty, int ind, Planet *plan);
int rad_tiny(int tnum, int tqty, Planet *plan);
int tmp_tiny(int tnum, int tqty, Planet *plan);


Tech::Tech(int tp, const char *nm, const char *ds, int res, int ind,
	int p1, int p2, int p3, int p4) {
  type = tp;  name = (char *)nm;  desc = (char *)ds;
  rcost = res;  icost = ind;
  prereq1 = p1;  prereq2 = p2;  prereq3 = p3;  prereq4 = p4;
  for(int ctr=0; ctr<max_factions; ++ctr) {
    known.push_back(base_tech/rcost);
    }
  }

TechTree::TechTree(int which, int techl) {
  max_factions = 64;
  init_tiny(techl);
  }

int TechTree::Minerals(int tnum, int tqty, Planet *plan) {
  return min_tiny(tnum, tqty, plan);
  }

int TechTree::Atmosphere(int tnum, int tqty, Planet *plan) {
  return atm_tiny(tnum, tqty, plan);
  }

int TechTree::Industry(int tnum, int tqty, int ind, Planet *plan) {
  return ind_tiny(tnum, tqty, ind, plan);
  }

int TechTree::Radiation(int tnum, int tqty, Planet *plan) {
  return rad_tiny(tnum, tqty, plan);
  }

int TechTree::Temperature(int tnum, int tqty, Planet *plan) {
  return tmp_tiny(tnum, tqty, plan);
  }

#define	TECH_CITIES		0
#define	TECH_ENTERTAINMENT	1
#define	TECH_PROPOGANDA		2
#define	TECH_LINARMINING	3
#define	TECH_SYSTEMFIGHTERS	4
#define	TECH_COLONYSHIPS	5

void TechTree::init_tiny(int techl) {
  base_tech = 25*(techl+1);
  lst.push_back(new Tech(TECH_PROJECT, "Cities",
    "Large, multifunction living and working spaces that require\n"
    "a population of one billion each to operate.",
    5, 100, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_PROJECT, "Entertainment",
    "The use of Industry to make population happier and more at ease.",
    25, 10, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_PROJECT, "Propoganda",
    "The use of Industry to make population feel secure and be more loyal.",
    25, 10, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_PROJECT, "Lunar Mining",
    "The mining of the resources of a planet's natural satellites.",
    30, 100, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_SHIP, "System Fighters",
    "A heavy space fighter able to attack and defend within a star system.",
    50, 200, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_SHIP, "Colony Ships",
    "A massive ship containing a portable self-contained city.",
    150, 500, TECH_CITIES, -1, -1, -1));

  num_sciences = 0;
  num_techs = 6;
  }

int min_tiny(int tnum, int tqty, Planet *plan) {
  switch(tnum) {
    case(TECH_CITIES): return -tqty;
    case(TECH_LINARMINING): return (plan->num_satellites <? tqty);
    }
  return 0;
  }

int atm_tiny(int tnum, int tqty, Planet *plan) {
  switch(tnum) {
    case(TECH_CITIES): return -((tqty+2)/3);
    }
  return 0;
  }

int ind_tiny(int tnum, int tqty, int ind, Planet *plan) {
  switch(tnum) {
    case(TECH_CITIES): return plan->Minerals()*tqty;
    case(TECH_LINARMINING): return -(plan->num_satellites <? tqty);
    case(TECH_ENTERTAINMENT): return -ind;
    case(TECH_PROPOGANDA): return -ind;
    }
  return 0;
  }

int rad_tiny(int tnum, int tqty, Planet *plan) {
  switch(tnum) {
    }
  return 0;
  }

int tmp_tiny(int tnum, int tqty, Planet *plan) {
  switch(tnum) {
    }
  return 0;
  }

#endif
