//#include <cstdio> //FIXME - NOT NEEDED
#include <cmath>

using namespace std;

#include "game.h"
#include "fleet.h"
#include "gui.h"

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

int Fleet::SMove() {            
  int ret = INT_MAX;
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ret = ret <? ships[ctr]->SMove();
    }
  return ret;
  }

int Fleet::GMove() {
  int ret = INT_MAX;
  for(int ctr=0; ctr<int(ships.size()); ++ctr) {
    ret = ret <? ships[ctr]->GMove();
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


//FIXME - HAXXOR
extern int panel;  // From gui.cpp
extern SObject *cur_object;  // From gui.cpp
void panel_draw();  // From gui.cpp

void Fleet::RemoveShip(int n) {
  ships.erase(ships.begin()+n);
  if(ships.size() <= 0) {
    Trash(this);
    if(cur_object == this) {
      cur_object = NULL;
      panel = PANEL_GAME;
      panel_draw();
      }
    }
  }

void Fleet::RemoveShips(int s, int e) {
  if(e < 0) e = int(ships.size());
  ships.erase(ships.begin()+s, ships.begin()+e);
  if(ships.size() <= 0) {
    Trash(this);
    if(cur_object == this) {
      cur_object = NULL;
      panel = PANEL_GAME;
      panel_draw();
      }
    }
  }

void Fleet::DestroyShip(int n) {
  delete ships[n];
  ships.erase(ships.begin()+n);
  if(ships.size() <= 0) {
    Trash(this);
    if(cur_object == this) {
      cur_object = NULL;
      panel = PANEL_GAME;
      panel_draw();
      }
    }
  }

void Fleet::DestroyShips(int s, int e) {
  if(e < 0) e = int(ships.size());
  for(int ctr=s; ctr<e; ++ctr) delete ships[ctr];
  ships.erase(ships.begin()+s, ships.begin()+e);
  if(ships.size() <= 0) {
    Trash(this);
    if(cur_object == this) {
      cur_object = NULL;
      panel = PANEL_GAME;
      panel_draw();
      }
    }
  }

int Fleet::Offense() {
  int ret=0;
  for(int ctr=0; ctr<int(ships.size()); ++ctr) ret += ships[ctr]->Offense();
  return ret>?1;
  }

int Fleet::Defense() {
  int ret=0;
  for(int ctr=0; ctr<int(ships.size()); ++ctr) ret += ships[ctr]->Defense();
  return ret>?1;
  }

void Fleet::Attack(Fleet *f) {
  int o1 = Offense();
  int o2 = f->Offense();
  int d1 = Defense();
  int d2 = f->Defense();
  int rounds = (d2+o1-1)/o1 <? (d1+o2-1)/o2;
  if((d2+o1-1)/o1 <= (d1+o2-1)/o2) {
    f->DestroyShips(0);
    int losses = NumShips() <? o2 * rounds * NumShips() / d1;
    //printf("Win, lose %d/%d\n", losses, NumShips());
    DestroyShips(NumShips()-losses);
    }
  else {
    DestroyShips(0);
    int losses = f->NumShips() <? o1 * rounds * f->NumShips() / d2;
    //printf("Lose, took out %d/%d\n", losses, f->NumShips());
    f->DestroyShips(f->NumShips()-losses);
    }
  }
