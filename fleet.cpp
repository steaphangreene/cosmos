#include <cmath>

using namespace std;

#include "game.h"
#include "fleet.h"

Fleet::Fleet(SObject *s, const char *nm) : SObject(s) {
  name = nm;
  frame = 0;
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

int Fleet::CanMoveS() {            
  int ret = INT_MAX;
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ret = ret <? ships[ctr]->CanMoveS();
    }
  return ret;
  }

int Fleet::CanMoveG() {
  int ret = INT_MAX;
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ret = ret <? ships[ctr]->CanMoveG();
    }
  return ret;
  }

int Fleet::Owner() {
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    if(ships[ctr]->Owner() >= 0) return ships[ctr]->Owner();
    }
  return -1;
  }

int Fleet::NumShips() {
  return int(ships.size());
  }

void Fleet::AddShip(Ship *s) {
  if(!s) return;
  if(s->Owner() >= 0) {
    seen[s->Owner()] = 1;
    known[s->Owner()] = 1;
    }
  ships.push_back(s);
  }

Ship *Fleet::GetShip(int n) {
  return ships[n];
  }

void Fleet::RemoveShip(int n) {
  ships.erase(ships.begin()+n);
  if(ships.size() <= 0) cur_game->junk.push_back(this);
  }

void Fleet::RemoveShips(int s, int e) {
  if(e < 0) e = int(ships.size());
  ships.erase(ships.begin()+s, ships.begin()+e);
  if(ships.size() <= 0) cur_game->junk.push_back(this);
  }
