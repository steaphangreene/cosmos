#include <cstdio>
#include <cstdlib>

#include "galaxy.h"

Galaxy::Galaxy(int numsys, int minl, int atmosl, int *devl, const vector<Player *> &pl) {
  num_systems = numsys;
  systems = new (System*)[numsys];
  for(int ctr=0; ctr<numsys; ++ctr) {
    int xpos = 34+rand()%700;
    int ypos = 34+rand()%700;
    for(int ctr2=0; ctr2<ctr; ++ctr2) {
      int offx = abs(xpos - systems[ctr2]->xpos);
      int offy = abs(ypos - systems[ctr2]->ypos);
      if(offx*offx + offy*offy < 1600) { xpos = 0; break; }
      }
    if(xpos == 0) { --ctr; continue; }  //Abort - too close;
    if(ctr < int(pl.size())) {
      systems[ctr] = new System(xpos, ypos, 10, minl, atmosl, devl, ctr);
      }
    else {
      systems[ctr] = new System(xpos, ypos, rand()%10, minl, atmosl, devl);
      }
    }
  }

void Galaxy::TakeTurn() {
  for(int ctr=0; ctr<num_systems; ++ctr) systems[ctr]->TakeTurn();  
  }
