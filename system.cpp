#include <cstdio>
#include <cstdlib>

#include "math.h"

#include "dict.h"
#include "game.h"
#include "system.h"

extern int max_factions;

System::System(int xp, int yp, int nump, int minl, int atmosl, int pl)
	: SObject(this) {
  name = dict[rand()%dict_size];
  for(int ctr=0; ctr<nump; ++ctr) {
    Planet *plan;
    if(pl >= 0 && ctr == 2) {
      plan = new Planet(this, ctr, (rand()%10000)+1, 30, 30);
      objects.push_back(plan);
      plan->colonies.push_back(new Colony(pl, plan, 1));
      }
    else {
      int at=0, mi=0;
      if(atmosl == 3) at = int(double(sqrt(double(rand()%901))));
      else if(atmosl == 2) at = rand()%30;
      else if(atmosl == 1) at = (rand()%30)*(rand()%30)/30;
      if(minl == 3) mi = int(double(sqrt(double(rand()%901))));
      else if(minl == 2) mi = rand()%30;
      else if(minl == 1) mi = (rand()%30)*(rand()%30)/30;
      plan = new Planet(this, ctr, (rand()%10000)+1, mi, at);
      objects.push_back(plan);
      }
    }
  if(pl >= 0) Know(pl);
  gxloc = xp;
  gyloc = yp;
  gxpos = (xp+10)/20;
  gypos = (yp+10)/20;
  }

System::~System() {
  for(int ctr=0; ctr<int(objects.size()); ++ctr) delete objects[ctr];
  objects.clear();
  }

void System::TakeTurn() {
  SObject::TakeTurn();
  for(int ctr=0; ctr<int(objects.size()); ++ctr) objects[ctr]->TakeTurn();
  }

int System::Owner() {
  for(int ctr=0; ctr<int(objects.size()); ++ctr) {
    if(objects[ctr]->SType() == SOBJECT_PLANET
		&& ((Planet*)objects[ctr])->colonies.size() > 0)
      return ((Planet*)objects[ctr])->colonies[0]->Owner();
    }
  return -1;
  }

void System::FleetLeaves(Fleet *f) {
  vector<SObject*>::iterator cur = objects.begin();
  while(cur < objects.end()) {   
    if(*cur == f) {
      cur = objects.erase(cur);
      continue;
      }
    ++cur;
    }
  }

void System::FleetArrives(Fleet *f) {
  vector<SObject*>::iterator cur = objects.begin();
  while(cur < objects.end()) {
    if(*cur == f) return;
    ++cur;
    }
  objects.push_back(f);
  Know(f->Owner());
  }

void System::Know(int n) {
  SObject::Know(n);
  for(int ctr=0; ctr<int(objects.size()); ++ctr) {
    objects[ctr]->See(n);
    }
  }

System::System(FILE *f) : SObject(0) {
  LoadFrom(f);
  }

void System::SaveTo(FILE *f) {
  SObject::SaveTo(f);
  fprintf(f, "%s\n", name.c_str());
  fprintf(f, "Objects: %d\n", int(objects.size()));
  for(int ctr=0; ctr<int(objects.size()); ++ctr) SaveSObject(f, objects[ctr]);
  }

void System::LoadFrom(FILE *f) {
  for(int ctr=0; ctr<int(objects.size()); ++ctr) delete objects[ctr];
  objects.clear();

  cur_system = this;
  SObject::LoadFrom(f);

  int tmpsz;
  system = this;
  char buf[1024] = {0};
  fscanf(f, "%[^\n]\n", buf); name = buf;
  fscanf(f, "Objects: %d\n", &tmpsz);
  for(int ctr=0; ctr<tmpsz; ++ctr) objects.push_back(LoadSObject(f));
  }
