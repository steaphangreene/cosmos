#include <cmath>

using namespace std;

#include "game.h"
#include "fleet.h"

Fleet::Fleet(SObject *s, int o, const char *nm) : SObject(s) {
  name = nm;
  owner = o;
  frame = 0;
  seen[o] = 1;
  known[o] = 1;
  }

Fleet::~Fleet() {
  vector<SObject *>::iterator cur;
  if(system) {
    cur = system->objects.begin();
    while(cur < system->objects.end()) {
      if(*cur == this) {
	cur = system->objects.erase(cur);
	continue;
	}
      ++cur;
      }
    }
  }

void Fleet::TakeTurn() {
  SObject::TakeTurn();
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ships[ctr]->TakeTurn();
    }
  }

int Fleet::CanLand() {
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    if(ships[ctr]->CanLand()) return 1;
    }
  return 0;
  }
