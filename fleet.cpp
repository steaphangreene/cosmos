#include "game.h"
#include "fleet.h"

Fleet::Fleet(int o, const char *nm) {
  name = nm;
  owner = o;
  frame = 0;
  cur_game->fleets.push_back(this);
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
  }

void Fleet::TakeTurn() {
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ships[ctr]->TakeTurn();
    }
  }
