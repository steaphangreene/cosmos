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
  name = sclass_name[sclass];
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

int Ship::CanLand() {
  if(sclass <= SCLASS_COLONIZER || sclass == SCLASS_COLONYSHIP) return 1;
  return 0;
  }
