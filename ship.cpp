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

int Ship::CanMoveS() {
  switch(sclass) {
    case(SCLASS_FIGHTER):
    case(SCLASS_TRANSPORT):
    case(SCLASS_COLONIZER):
    case(SCLASS_COLONYSHIP):
    case(SCLASS_DESTROYER): {
      return 1;
      } break;
    case(SCLASS_MAX): {
      //Should never get here
      } break;
    }
  return 0;
  }

int Ship::CanMoveG() {
  switch(sclass) {
    case(SCLASS_FIGHTER):
    case(SCLASS_TRANSPORT):
    case(SCLASS_COLONIZER): {
      return 0;
      } break;
    case(SCLASS_COLONYSHIP): {
      return 50000;
      } break;
    case(SCLASS_DESTROYER): {
      return 100000;
      } break;
    case(SCLASS_MAX): {
      //Should never get here
      } break;
    }
  return 0;
  }
