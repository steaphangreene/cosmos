#include <cstdio>

#include "planet.h"
#include "tech_local.h"

extern int base_tech;

#define	TECH_TINY_CITIES		0
#define	TECH_TINY_ENTERTAINMENT		1
#define	TECH_TINY_PROPOGANDA		2
#define	TECH_TINY_LINARMINING		3
#define	TECH_TINY_FIGHTERS		4
#define	TECH_TINY_COLONYSHIPS		5

void TechTree::init_tiny(int techl) {
  base_tech = 25*(techl+1);
  lst.push_back(new Tech(TECH_STRUCTURE, "City", "Cities",
	"Large, multifunction living and working spaces that require\n"
	"a population of one billion each to operate.",
	5, 100, 20, 1000, 0, 0, 0,
	-1, -1, -1, -1));
  lst.push_back(new Tech(TECH_PROJECT, "Entertainment", "Entertainment",
	"The use of Industry to make population happier and more at ease.",
	25, 10, 10, 10, 0, 0, -1,
	-1, -1, -1, -1));
  lst.push_back(new Tech(TECH_PROJECT, "Propoganda", "Propoganda",
	"The use of Industry to make population feel secure and be more loyal.",
	25, 10, 10, 10, -1, -1, 0,
	-1, -1, -1, -1));
  lst.push_back(new Tech(TECH_STRUCTURE, "Sat Mine", "Sat Mines",
	"The mining of the resources of a planet's natural satellites.",
	30, 100, 10, 100, 0, 0, 0,
	-1, -1, -1, -1));
  lst.push_back(new Tech(TECH_SHIP, "Fighter", "Fighters",
	"A heavy space fighter able to attack and defend within a star system.",
	50, 200, 50, 1, 10, 10, 0,
	-1, -1, -1, -1));
  lst.push_back(new Tech(TECH_SHIP, "Colony Ship", "Colony Ships",
	"A massive ship containing a portable self-contained city.",
	150, 500, 100, 1000, 0, 10, 10,
	TECH_TINY_CITIES, -1, -1, -1));

  num_sciences = 0;
  num_techs = 6;
  }

int min_tiny(int tnum, int tqty, Planet *plan) {
  switch(tnum) {
    case(TECH_TINY_CITIES): return -tqty;
    case(TECH_TINY_LINARMINING): return ((plan->num_satellites ? 4:0) <? tqty);
    }
  return 0;
  }

int atm_tiny(int tnum, int tqty, Planet *plan) {
  switch(tnum) {
    case(TECH_TINY_CITIES): return -((tqty+2)/3);
    }
  return 0;
  }

int ind_tiny(int tnum, int tqty, Planet *plan) {
  switch(tnum) {
    case(TECH_TINY_CITIES): return plan->Minerals()*tqty;
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

int upk_tiny(int tnum, int tqty, Planet *plan) {
  int ret = cur_tree->GetTech(tnum)->upkeep * tqty;
  ret = tech_reduce(ret, cur_tree->GetTech(tnum)->known[plan->claimed]);
  return ret;
  }

int crw_tiny(int tnum, int tqty, Planet *plan) {
  int ret = cur_tree->GetTech(tnum)->crew * tqty;
  return ret;
  }

int loy_tiny(int tnum, int tqty, Planet *plan) {
  int ret = cur_tree->GetTech(tnum)->loyalty * tqty;
  if(ret < 0) {
    ret = (-ret) * cur_tree->GetTech(tnum)->known[plan->claimed];
    }
  return ret;
  }

int sec_tiny(int tnum, int tqty, Planet *plan) {
  int ret = cur_tree->GetTech(tnum)->security * tqty;
  if(ret < 0) {
    ret = (-ret) * cur_tree->GetTech(tnum)->known[plan->claimed];
    }
  return ret;
  }

int hap_tiny(int tnum, int tqty, Planet *plan) {
  int ret = cur_tree->GetTech(tnum)->happiness * tqty;
  if(ret < 0) {
    ret = (-ret) * cur_tree->GetTech(tnum)->known[plan->claimed];
    }
  return ret;
  }
