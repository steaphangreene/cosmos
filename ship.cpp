#include "dict.h"
#include "ship.h"
#include "techtree.h"

const char *sclass_name[SCLASS_MAX] = {
  "Transport",
  "Colonizer",
  "Fighter",
  "Colony Ship",
  "Destroyer"
  };

Ship::Ship(int tec, int own) {
  tech = tec;
  owner = own;
  Tech *tc = cur_tree->GetTech(tech);
  sclass = SClass(tc->special);
  name = dict[rand()%dict_size];
  mincrew = tc->crew;
  maxcrew = tc->crew;
  crew = 0;
  switch(sclass) {
    case(SCLASS_TRANSPORT): {
      } break;
    case(SCLASS_COLONIZER): {
      } break;
    case(SCLASS_COLONYSHIP): {
      } break;
    case(SCLASS_FIGHTER): {
      } break;
    case(SCLASS_DESTROYER): {
      } break;
    case(SCLASS_MAX): {
      //Should never get here
      } break;
    }
  }

void Ship::TakeTurn() {
  }

const char *Ship::CName() {
  return sclass_name[sclass];
  }

int Ship::CanLand() {
  if(sclass <= SCLASS_COLONIZER || sclass == SCLASS_COLONYSHIP) return 1;
  return 0;
  }

int Ship::SMove() {
  switch(sclass) {
    case(SCLASS_FIGHTER):	return 500;
    case(SCLASS_TRANSPORT):	return 200;
    case(SCLASS_COLONIZER):	return 100;
    case(SCLASS_COLONYSHIP):	return 100;
    case(SCLASS_DESTROYER):	return 300;
    case(SCLASS_MAX):break;
    }
  return 0;
  }

int Ship::GMove() {
  switch(sclass) {
    case(SCLASS_FIGHTER):	return 0;
    case(SCLASS_TRANSPORT):	return 0;
    case(SCLASS_COLONIZER):	return 0;
    case(SCLASS_COLONYSHIP):	return 100;
    case(SCLASS_DESTROYER):	return 300;
    case(SCLASS_MAX):break;
    }
  return 0;
  }

int Ship::Offense() {
  switch(sclass) {
    case(SCLASS_FIGHTER):	return 100;
    case(SCLASS_TRANSPORT):	return 1;
    case(SCLASS_COLONIZER):	return 5;
    case(SCLASS_COLONYSHIP):	return 5;
    case(SCLASS_DESTROYER):	return 150;
    case(SCLASS_MAX):break;
    }
  return 1;
  }

int Ship::Defense() {
  switch(sclass) {
    case(SCLASS_FIGHTER):	return 50;
    case(SCLASS_TRANSPORT):	return 50;
    case(SCLASS_COLONIZER):	return 250;
    case(SCLASS_COLONYSHIP):	return 250;
    case(SCLASS_DESTROYER):	return 100;
    case(SCLASS_MAX):break;
    }
  return 1;
  }

Ship::Ship(FILE *f) {
  LoadFrom(f);
  }

void Ship::SaveTo(FILE *f) {
  fprintf(f, "%s\n", name.c_str());
  fprintf(f, "%d %d %d %d %d %d\n",
	tech, (int)sclass, mincrew, maxcrew, crew, owner);
  }

void Ship::LoadFrom(FILE *f) {
  char buf[1024] = {0};
  fscanf(f, "%[^\n]\n", buf);  name = buf;
  fscanf(f, "%d %d %d %d %d %d\n",
	&tech, (int*)&sclass, &mincrew, &maxcrew, &crew, &owner);
  }

