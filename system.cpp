#include <cstdio>
#include <cstdlib>

#include "math.h"

#include "system.h"

extern int max_factions;

System::System(int xp, int yp, int nump, int minl, int atmosl, int pl) {
  num_planets = nump;
  planets = new (Planet*)[nump];
  explored.resize(max_factions, 0);
  for(int ctr=0; ctr<nump; ++ctr) {
    if(pl >= 0 && ctr == 2) {
      planets[ctr] = new Planet(this, ctr, (rand()%10000)+1, 30, 30);
      planets[ctr]->colonies.push_back(new Colony(pl, planets[ctr], 1));
      Explore(pl);
      }
    else {
      int at=0, mi=0;
      if(atmosl == 3) at = int(double(sqrt(double(rand()%901))));
      else if(atmosl == 2) at = rand()%30;
      else if(atmosl == 1) at = (rand()%30)*(rand()%30)/30;
      if(minl == 3) mi = int(double(sqrt(double(rand()%901))));
      else if(minl == 2) mi = rand()%30;
      else if(minl == 1) mi = (rand()%30)*(rand()%30)/30;
      planets[ctr] = new Planet(this, ctr, (rand()%10000)+1, mi, at);
      }
    }
  xpos = xp;
  ypos = yp;
  }

System::~System() {
  for(int ctr=0; ctr < num_planets; ++ctr) {
    delete planets[ctr];
    }
  delete [] planets;
  planets = NULL;
  }

void System::TakeTurn() {
  for(int ctr=0; ctr<num_planets; ++ctr) planets[ctr]->TakeTurn();
  }

void System::FleetLeaves(Fleet *f) {
  vector<Fleet *>::iterator cur = fleets.begin();
  while(cur < fleets.end()) {
    if(*cur == f) {
      cur = fleets.erase(cur);
      continue;
      }
    ++cur;
    }
  }

void System::FleetArrives(Fleet *f) {
  vector<Fleet *>::iterator cur = fleets.begin();
  while(cur < fleets.end()) {
    if(*cur == f) return;
    ++cur;
    }
  fleets.push_back(f);
  Explore(f->Owner());
  }

int System::Owner() {
  for(int ctr=0; ctr < num_planets; ++ctr) {
    if(planets[ctr]->colonies.size() > 0)
      return planets[ctr]->colonies[0]->Owner();
    }
  return -1;
  }

int System::ExploredBy(int n) {
  return explored[n];
  }

void System::Explore(int n) {
  explored[n] = 1;
  }
