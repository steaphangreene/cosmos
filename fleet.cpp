#include <cmath>

using namespace std;

#include "game.h"
#include "fleet.h"

extern int max_factions;

Fleet::Fleet(SObject *s, int o, const char *nm) : SObject(s) {
  detected.resize(max_factions, 0);
  detected[o] = 1;
  name = nm;
  owner = o;
  frame = 0;
  targ = NULL;
  dist = -1;
  }

Fleet::~Fleet() {
  vector<SObject *>::iterator cur;
  if(system) {
    cur = system->fleets.begin();
    while(cur < system->fleets.end()) {
      if(*cur == this) {
	cur = system->fleets.erase(cur);
	continue;
	}
      ++cur;
      }
    }
  }

void Fleet::TakeTurn() {
  targ = NULL;
  dist = -1;
  SObject::TakeTurn();
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
  destination = targ;
  depart_turn = cur_game->turn;
  arrive_turn = cur_game->turn + dist;
  if(arrive_turn == depart_turn) Arrive();
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
