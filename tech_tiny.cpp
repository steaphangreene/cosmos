#include <cstdio>

#include "ship.h"
#include "planet.h"
#include "tech_local.h"

extern int base_tech;

enum {	TECH_TINY_CITY = 0,
	TECH_TINY_ENTERTAINMENT,
	TECH_TINY_PROPOGANDA,
	TECH_TINY_SATMINE,
	TECH_TINY_COLONIZER,
	TECH_TINY_FIGHTER,
	TECH_TINY_COLONYSHIP,
	TECH_TINY_DESTROYER,
	TECH_TINY_MAX };

static int start[TECH_TINY_MAX] = {0};

void TechTree::init_tiny(int techl, int devl) {
  base_tech = 25*(techl+1);
  devl += 1; //Make it 1-based
  start[lst.size()] = 3*devl;
  lst.push_back(new Tech(TECH_STRUCTURE, "City", "Cities",
	"Large, multifunction living and working spaces that require\n"
	"a population of one billion each to operate.",
	5, 100, 10, 500, 0, 0, 0, 0,
	-1, -1, -1, -1));
  start[lst.size()] = 1;
  lst.push_back(new Tech(TECH_PROJECT, "Entertainment", "Entertainment",
	"The use of Industry to make population happier and more at ease.",
	25, 10, 10, 10, 0, 0, -1, 0,
	-1, -1, -1, -1));
  start[lst.size()] = 1;
  lst.push_back(new Tech(TECH_PROJECT, "Propoganda", "Propoganda",
	"The use of Industry to make population feel secure and be more loyal.",
	25, 10, 10, 10, -1, -1, 0, 0,
	-1, -1, -1, -1));
  start[lst.size()] = 4;
  lst.push_back(new Tech(TECH_STRUCTURE, "Sat Mine", "Sat Mines",
	"The mining of the resources of a planet's natural satellites.",
	30, 100, 10, 100, 0, 0, 0, 0,
	-1, -1, -1, -1));
  start[lst.size()] = 0;
  lst.push_back(new Tech(TECH_SHIP, "Transport", "Transports",
	"A large, cheap one-use ship used to transport 100 Million people\n"
	"from one colony to another within a system.",
	50, 10, 4, 100, 0, 0, 0, SCLASS_TRANSPORT,
	TECH_TINY_CITY, -1, -1, -1));
  start[lst.size()] = 1;
  lst.push_back(new Tech(TECH_SHIP, "Colonizer", "Colonizers",
	"A massive system ship containing a portable self-contained city\n"
	"which can be landed on another planet in the system to establish\n"
	"or enhance a colony.",
	75, 200, 4, 1000, 0, 10, 10, SCLASS_COLONIZER,
	TECH_TINY_CITY, -1, -1, -1));
  start[lst.size()] = 2;
  lst.push_back(new Tech(TECH_SHIP, "Fighter", "Fighters",
	"A heavy space fighter able to attack and defend within a star system.",
	50, 200, 1, 1, 10, 10, 0, SCLASS_FIGHTER,
	-1, -1, -1, -1));
  start[lst.size()] = 0;
  lst.push_back(new Tech(TECH_SHIP, "Colony Ship", "Colony Ships",
	"A massive hyperspace ship containing a portable self-contained city\n"
	"which can be landed on a planet to establish or enhance a colony.",
	150, 1000, 10, 500, 0, 10, 10, SCLASS_COLONYSHIP,
	TECH_TINY_CITY, TECH_TINY_COLONIZER, -1, -1));
  start[lst.size()] = 0;
  lst.push_back(new Tech(TECH_SHIP, "Destroyer", "Destroyers",
	"A light hyperspace warship.",
	100, 500, 6, 1, 10, 10, 0, SCLASS_DESTROYER,
	TECH_TINY_FIGHTER, -1, -1, -1));

  num_sciences = 0;
  num_techs = int(lst.size()) - num_sciences;
  homeworld = start;
  }

int min_tiny(int tnum, int tqty, Colony *col) {
  switch(tnum) {
    case(TECH_TINY_CITY): return -tqty;
    case(TECH_TINY_SATMINE): {
      if(col->Plan() == NULL) return 0;
      return ((col->Plan()->num_satellites ? 4:0) <? tqty);
      }
    }
  return 0;
  }

int atm_tiny(int tnum, int tqty, Colony *col) {
  switch(tnum) {
    case(TECH_TINY_CITY): return -((tqty+2)/3);
    }
  return 0;
  }

int ind_tiny(int tnum, int tqty, Colony *col) {
  switch(tnum) {
    case(TECH_TINY_CITY): {
      if(col->Plan() == NULL) return 0;
      return col->Plan()->Minerals()*tqty;
      }
    }
  return 0;
  }

int rad_tiny(int tnum, int tqty, Colony *col) {
  switch(tnum) {
    }
  return 0;
  }

int tmp_tiny(int tnum, int tqty, Colony *col) {
  switch(tnum) {
    }
  return 0;
  }

int upk_tiny(int tnum, int tqty, Colony *col) {
  int ret = cur_tree->GetTech(tnum)->upkeep * tqty;
  ret = tech_reduce(ret, cur_tree->GetTech(tnum)->known[col->Owner()]);
  return ret;
  }

int crw_tiny(int tnum, int tqty, Colony *col) {
  int ret = cur_tree->GetTech(tnum)->crew * tqty;
  return ret;
  }

int loy_tiny(int tnum, int tqty, Colony *col) {
  int ret = cur_tree->GetTech(tnum)->loyalty * tqty;
  if(ret < 0) {
    ret = (-ret) * cur_tree->GetTech(tnum)->known[col->Owner()];
    }
  return ret;
  }

int sec_tiny(int tnum, int tqty, Colony *col) {
  int ret = cur_tree->GetTech(tnum)->security * tqty;
  if(ret < 0) {
    ret = (-ret) * cur_tree->GetTech(tnum)->known[col->Owner()];
    }
  return ret;
  }

int hap_tiny(int tnum, int tqty, Colony *col) {
  int ret = cur_tree->GetTech(tnum)->happiness * tqty;
  if(ret < 0) {
    ret = (-ret) * cur_tree->GetTech(tnum)->known[col->Owner()];
    }
  return ret;
  }
