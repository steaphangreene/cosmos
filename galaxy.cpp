#include <cstdio>
#include <cstdlib>

#include "game.h"
#include "galaxy.h"

Galaxy::Galaxy(int numsys, int minl, int atmosl, const vector<Player *> &pl) {
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
      systems.push_back(new System(xpos, ypos, 10, minl, atmosl, ctr));
      }
    else {
      systems.push_back(new System(xpos, ypos, rand()%10, minl, atmosl));
      }
    }
  }

void Galaxy::TakeTurn() {
  for(int ctr=0; ctr<int(systems.size()); ++ctr) systems[ctr]->TakeTurn();  
  }

Galaxy::~Galaxy() {
  for(int ctr=0; ctr<int(systems.size()); ++ctr) delete systems[ctr];
  systems.clear();
  }

Galaxy::Galaxy(FILE *f) {
  LoadFrom(f);
  }

void Galaxy::SaveTo(FILE *f) {
  fprintf(f, "Systems: %d\n", int(systems.size()));
  for(int ctr=0; ctr<int(systems.size()); ++ctr) systems[ctr]->SaveTo(f);
  }

void Galaxy::LoadFrom(FILE *f) {
  for(int ctr=0; ctr<int(systems.size()); ++ctr) delete systems[ctr];
  systems.clear();

  cur_galaxy = this;
  int tmpsz;
  fscanf(f, "Systems: %d\n", &tmpsz);
  for(int ctr=0; ctr<tmpsz; ++ctr) systems.push_back(new System(f));
  }
