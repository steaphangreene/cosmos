#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "game.h"
#include "ship.h"

Colony::Colony(int o, Planet *p, int initial) {
  planet = p;
  owner  = o;
  Init(initial);
  }

void Colony::Init(int initial) {
  population = 0;
  pop_minor = 0;
  if(initial) population = 500;
  for(int ctr=0; ctr<cur_tree->NumTechs(); ++ctr) {
    Tech *tc = cur_tree->GetTech(ctr);
    if(tc->type == TECH_STRUCTURE && tc->known[owner]) {
      objs.push_back(ctr);
      oqty.push_back(initial * cur_tree->Homeworld(ctr));
      population += initial * tc->crew * cur_tree->Homeworld(ctr);
      }
    }
  for(int ctr=0; ctr<cur_tree->NumTechs(); ++ctr) {
    Tech *tc = cur_tree->GetTech(ctr);
    if(tc->type == TECH_PROJECT && tc->known[owner]) {
      objs.push_back(ctr);
      oqty.push_back(initial * cur_tree->Homeworld(ctr));
      population += initial * tc->crew * cur_tree->Homeworld(ctr);
      }
    }
  int fn = 0;
  if(!planet) return;
  planet->See(owner);
  planet->Know(owner);
  for(int ctr=0; ctr<cur_tree->NumTechs(); ++ctr) {
    Tech *tc = cur_tree->GetTech(ctr);
    if(tc->type == TECH_SHIP) {
      objs.push_back(ctr);
      oqty.push_back(0);
      if(initial && cur_tree->Homeworld(ctr) > 0) {
	Fleet *flt = new Fleet(planet, owner, tc->names);
	planet->Sys()->objects.push_back(flt);
	for(int shp=0 ; shp < cur_tree->Homeworld(ctr); ++shp) {
	  Ship *shp = new Ship(ctr, owner);
	  flt->ships.push_back(shp);
	  shp->AddCrew(shp->MaxCrew());
	  }
	++fn;
	}
      }
    }
  }

Colony::~Colony() {
  }

int Colony::Industry() {
  int ind = Population()/1000;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    ind += cur_tree->Industry(objs[ctr], oqty[ctr], this);
    }
  if(planet) {
    for(int flt=0; flt<int(planet->Sys()->objects.size()); ++flt) {
      if(planet->Sys()->objects[flt]->SType() == SOBJECT_FLEET) {
	for(int shp=0; shp<int(((Fleet*)planet->Sys()->objects[flt])->ships.size()); ++shp) {
	  ind += cur_tree->Industry(((Fleet*)planet->Sys()->objects[flt])->ships[shp]->tech, 1, this);
	  }
	}
      }
    }
  long long free;
  free = FreePop();
  free *= 1000000;
  free += FreePopM();
  if(free < 0) {
    long long tmp = ind, need, busy;
    busy = Population();
    busy *= 1000000;
    busy += PopulationM();
    need = busy + (-free);
    tmp *= busy;
    tmp /= need;
    ind = tmp;
    }
  return 0 >? ind;
  }

int Colony::SpareIndustry() {
  int ind = Industry();
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    ind -= cur_tree->Upkeep(objs[ctr], oqty[ctr], this);
    }
  return 0 >? ind;
  }

int Colony::Population() {
  return population;
  }

int Colony::PopulationM() {
  return pop_minor;
  }

int Colony::FreePop() {
  int pop = population;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    pop -= cur_tree->Crew(objs[ctr], oqty[ctr], this);
    }
  return pop;
  }

int Colony::FreePopM() {
  if(pop_minor == 0) return 0;
  else if(FreePop() < 0) return -1000000 + pop_minor;
  else return pop_minor;
  }

int Colony::Growth() {
  long long ret = population;
  ret *= Happiness();
  ret *= Security();
  ret /= 100;
  ret /= 1000000;
  return ret;
  }

int Colony::GrowthM() {
  long long ret = population;
  ret *= Happiness();
  ret *= Security();
  ret /= 100;
  ret %= 1000000;
  return ret;
  }

int Colony::Happiness() {
  if(Population() == 0) return 0;
  int hap = FreePop()*100/Population();
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    hap += cur_tree->Happiness(objs[ctr], oqty[ctr], this);
    }
  if(planet) {
    for(int flt=0; flt<int(planet->Sys()->objects.size()); ++flt) {
      if(planet->Sys()->objects[flt]->SType() == SOBJECT_FLEET) {
	for(int shp=0; shp<int(((Fleet*)planet->Sys()->objects[flt])->ships.size()); ++shp) {
	  hap += cur_tree->Happiness(((Fleet*)planet->Sys()->objects[flt])->ships[shp]->tech, 1, this);
	  }
	}
      }
    }
  return 0 >? hap <? 1000;
  }

int Colony::Security() {
  int sec = 0;
  for(int ctr=0; ctr<int(objs.size()); ++ctr) {
    sec += cur_tree->Security(objs[ctr], oqty[ctr], this);
    }
  if(planet) {
    for(int flt=0; flt<int(planet->Sys()->objects.size()); ++flt) {
      if(planet->Sys()->objects[flt]->SType() == SOBJECT_FLEET) {
	for(int shp=0; shp<int(((Fleet*)planet->Sys()->objects[flt])->ships.size()); ++shp) {
	  sec += cur_tree->Security(((Fleet*)planet->Sys()->objects[flt])->ships[shp]->tech, 1, this);
	  }
	}
      }
    }
  return 0 >? sec <? 1000;
  }

int Colony::Loyalty() {
  int loy = 0;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    loy += cur_tree->Loyalty(objs[ctr], oqty[ctr], this);
    }
  if(planet) {
    for(int flt=0; flt<int(planet->Sys()->objects.size()); ++flt) {
      if(planet->Sys()->objects[flt]->SType() == SOBJECT_FLEET) {
	for(int shp=0; shp<int(((Fleet*)planet->Sys()->objects[flt])->ships.size()); ++shp) {
	  loy += cur_tree->Loyalty(((Fleet*)planet->Sys()->objects[flt])->ships[shp]->tech, 1, this);
	  }
	}
      }
    }
  return loy;
  }

void Colony::TakeTurn() {
  pop_minor += GrowthM();
  population += pop_minor / 1000000;
  pop_minor %= 1000000;
  population += Growth();

  int ind = Industry();
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    int diff = cur_tree->Upkeep(objs[ctr], oqty[ctr], this);
    if(diff > ind) {
      --oqty[ctr];
      --ctr;
      continue;
      }
    ind -= diff;
    }

  if(SpareIndustry() > 0 && projs.size() > 0) {
    int indus = SpareIndustry(), need, ctr;
    Tech *tc;
    while(projs.size() > 0 && indus > 0) {
      tc = cur_tree->GetTech(projs[0]);
      need = tc->icost;
      if(prog[0]+indus >= need) {
	indus -= need-prog[0];
	if(tc->type == TECH_SHIP) {
	  if(planet) {
	    Fleet *flt = new Fleet(planet, owner, tc->names);
	    planet->Sys()->objects.push_back(flt);
	    flt->ships.push_back(new Ship(projs[0], owner));
	    flt->ships[0]->AddCrew(flt->ships[0]->MaxCrew());
	    population -= flt->ships[0]->MaxCrew();
	    //flt->location = planet;
	    }
          }
	else {
	  for(ctr=0; ctr < (int)objs.size(); ++ctr) {
	    if(objs[ctr] == projs[0]) { ++oqty[ctr]; break; }
	    }
	  if(ctr == (int)objs.size()) {
	    objs.push_back(projs[0]);
	    oqty.push_back(1);
	    }
	  }
	projs.erase(projs.begin());
	prog.erase(prog.begin());
	}
      else {
	prog[0] += indus;
	indus = 0;
	}
      }
    }
  }

void Colony::LandShip(Ship *s) {
  switch(s->Class()) {
    case(SCLASS_COLONYSHIP):
    case(SCLASS_COLONIZER): {
      for(int ctr=0; ctr < int(objs.size()); ++ctr) {
        if(objs[ctr] == 0) {	// 0 == CITY - FIXME!
	  ++oqty[ctr];
	  break;
	  }
	}
      }break;
    case(SCLASS_TRANSPORT): {
      }break;
    default: {
      fprintf(stderr, "How the fuck did you land THAT on a planet?!?\n");
      }break;
    }
  population += s->Crew();
  }
