#include "ship.h"
#include "techtree.h"

const char *sclass_name[SCLASS_MAX] = {
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
