#include <cstdio>
#include <cstdlib>

#include "game.h"
#include "galaxy.h"

Galaxy::Galaxy(int numsys, int minl, int atmosl, const vector<Player *> &pl) {
  num_systems = numsys;
  systems = new (System*)[numsys];
  for(int ctr=0; ctr<numsys; ++ctr) {
    int xpos = 34+rand()%14000;
    int ypos = 34+rand()%14000;
    for(int ctr2=0; ctr2<ctr; ++ctr2) {
      int offx = abs((xpos+10)/20 - systems[ctr2]->GXPos());
      int offy = abs((ypos+10)/20 - systems[ctr2]->GYPos());
      if(offx*offx + offy*offy < 1600) { xpos = 0; break; }
      }
    if(xpos == 0) { --ctr; continue; }  //Abort - too close;
    if(ctr < int(pl.size())) {
      systems[ctr] = new System(xpos, ypos, 10, minl, atmosl, ctr);
      }
    else {
      systems[ctr] = new System(xpos, ypos, rand()%10, minl, atmosl);
      }
    }
  }

void Galaxy::TakeTurn() {
  for(int ctr=0; ctr<num_systems; ++ctr) systems[ctr]->TakeTurn();  
  }
