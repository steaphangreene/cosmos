#include <cmath>

using namespace std;

#include "game.h"
#include "fleet.h"

extern int max_factions;

Fleet::Fleet(int o, const char *nm) {
  cur_game->fleets.push_back(this);
  detected.resize(max_factions, 0);
  detected[o] = 1;
  name = nm;
  owner = o;
  frame = 0;
  loc = NULL;
  dest = NULL;
  targ = NULL;
  prog = -1;
  trip = 0;
  dist = -1;
  }

Fleet::~Fleet() {
  vector<Fleet *>::iterator cur = cur_game->fleets.begin();
  while(cur < cur_game->fleets.end()) {
    if(*cur == this) {
      cur = cur_game->fleets.erase(cur);
      continue;
      }
    ++cur;
    }
  if(loc) {
    cur = loc->fleets.begin();
    while(cur < loc->fleets.end()) {
      if(*cur == this) {
	cur = loc->fleets.erase(cur);
	continue;
	}
      ++cur;
      }
    cur = loc->Sys()->fleets.begin();
    while(cur < loc->Sys()->fleets.end()) {
      if(*cur == this) {
	cur = loc->Sys()->fleets.erase(cur);
	continue;
	}
      ++cur;
      }
    }
  }

void Fleet::Arrive() {
  prog = -1;
  loc = dest;
  dest = NULL;
  loc->FleetArrives(this);
  loc->Sys()->FleetLeaves(this);
  }

void Fleet::TakeTurn() {
  targ = NULL;
  dist = -1;
  if(prog >= 0) {
    ++prog;
    if(prog >= abs(trip)) Arrive();
    }
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ships[ctr]->TakeTurn();
    }
  }

int Fleet::TimeToLocal(int sqdist) {
  return (int(sqrt(double(sqdist)))+19)/20;
  }

int Fleet::TimeToGalactic(int sqdist) {
  return TimeToLocal(sqdist)*1000000;
  }

void Fleet::Engage() {
  if(targ == NULL) return;
  dest = targ;
  trip = dist;
  loc->FleetLeaves(this);
  loc->Sys()->FleetArrives(this);
  prog = 0;
  if(trip == 0) Arrive();
  }

void Fleet::SetCourse(Planet *p, int d) {
  targ = p;
  dist = d;
  }

void Fleet::SetCourse(System *s, int d) {
//  targ = p;
  dist = d;
  }

int Fleet::CanLand() {
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    if(ships[ctr]->CanLand()) return 1;
    }
  return 0;
  }

int Fleet::DetectedBy(int n) {
  return detected[n];
  }

void Fleet::Detect(int n) {
  detected[n] = 1;
  }
