#include <cstdio>

#include "techtree.h"

static int base_tech;
static int max_factions;

Tech::Tech(int tp, const char *nm, const char *ds, int res, int ind,
	int p1, int p2, int p3, int p4) {
  type = tp;  name = (char *)nm;  desc = (char *)ds;
  rcost = res;  icost = ind;
  prereq1 = p1;  prereq2 = p2;  prereq3 = p3;  prereq4 = p4;
  for(int ctr=0; ctr<max_factions; ++ctr) {
    known.push_back(base_tech/rcost);
    }
  }

TechTree::TechTree(int which, int level) {
  base_tech = 25*(level+1);
  max_factions = 64;
  lst.push_back(new Tech(TECH_PROJECT, "Cities",
    "Large, multifunction living and working spaces that hold\n"
    "millions of population each.",
    5, 100, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_PROJECT, "Lunar Mining",
    "The mining of the resources of a planet's natural satellites.",
    30, 100, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_SHIP, "System Fighters",
    "A heavy space fighter able to attack and defend within a star system.",
    50, 200, -1, -1, -1, -1));
  lst.push_back(new Tech(TECH_SHIP, "Colony Ships",
    "The mining of the resources of a planet's natural satellites.",
    150, 500, -1, -1, -1, -1));
  num_sciences = 0;
  num_techs = 4;
  }
