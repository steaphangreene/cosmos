#include <cmath>

using namespace std;

#include "game.h"
#include "fleet.h"

Fleet::Fleet(int o, const char *nm) {
  cur_game->fleets.push_back(this);
  name = nm;
  owner = o;
  frame = 0;
  loc = NULL;
  dest = NULL;
  prog = 0;
  trip = 0;
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
  prog = 0;
  loc = dest;
  dest = NULL;
  loc->FleetArrives(this);
  loc->Sys()->FleetLeaves(this);
  }

void Fleet::TakeTurn() {
  if(dest) {
    ++prog;
    if(prog >= abs(trip)) Arrive();
    }
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ships[ctr]->TakeTurn();
    }
  }

int Fleet::TimeToLocal(int sqdist) {
  return (int(sqrt(double(sqdist))))+19/20;
  }

int Fleet::TimeToGalactic(int sqdist) {
  return -1020;
  }

void Fleet::SetCourse(Planet *p, int d) {
  loc->FleetLeaves(this);
  loc->Sys()->FleetArrives(this);
  dest = p;
  prog = 0;
  trip = d;
  if(trip == 0) Arrive();
  }

void Fleet::SetCourse(System *s, int d) {
//  dest = p;
  prog = 0;
  trip = d;
  if(trip == 0) Arrive();
  }

int Fleet::CanLand() {
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    if(ships[ctr]->CanLand()) return 1;
    }
  return 0;
  }
