#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "game.h"
#include "ship.h"

//Orbital Period = sqrt(Distance From Star ^ 3)

int sattp[5][5] = {
	{ },
	{ 0 },
	{ 1, 1 },
	{ 1, 2, 2 },
	{ 2, 2, 2, 2 },
	};

Planet::Planet(int ord, int sz, int min, int atmos) {
  order = ord;
  size = sz;
  minerals = min;
  population = 0;
  pop_minor = 0;
  atmosphere = atmos;
  temperature = (10-order)*(10-order)*4-240;
  radiation = (10-order)*(10-order)/2;
  claimed = -1;
  int dist = (ord+1)*700;
  period = int(sqrt(double(dist)*double(dist)*double(dist)));
  startpos = rand()&65535;
  num_satellites = (rand()%4)+1;
  satellites = new (Satellite*)[num_satellites];
  for(int ctr=0; ctr<num_satellites; ++ctr) {
    satellites[ctr] = new Satellite(rand()&65535, sattp[num_satellites][ctr]);
    }
  };

Planet::~Planet() {
  for(int ctr=0; ctr < num_satellites; ++ctr) {
    delete satellites[ctr];
    }
  delete [] satellites;
  satellites = NULL;
  }

int Planet::XPos(int turn) {
  double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
  double dist = double(order+1) * double(30);
  double xpos = dist * cos(ang * 2.0 * M_PIl / double(65536));
  return int(384+xpos);
  };

int Planet::YPos(int turn) {
  double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
  double dist = double(order+1) * double(30);
  double ypos = dist * sin(ang * 2.0 * M_PIl / double(65536));
  return int(384+ypos);
  };

int Planet::Type() {
  return 0;
  }

int Planet::Radiation() {
  int rad = radiation-Atmosphere();
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    rad += cur_tree->Radiation(objs[ctr], oqty[ctr], this);
    }
  return 0 >? rad;
  }

int Planet::Temperature() {
  int tmp = temperature;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    tmp += cur_tree->Temperature(objs[ctr], oqty[ctr], this);
    }
  return 0 >? tmp;
  }

int Planet::Atmosphere() {
  int atm = atmosphere;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    atm += cur_tree->Atmosphere(objs[ctr], oqty[ctr], this);
    }
  return 0 >? atm;
  }

int Planet::Minerals() {
  int min = minerals;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    min += cur_tree->Minerals(objs[ctr], oqty[ctr], this);
    }
  return 0 >? min;
  }

int Planet::Industry() {
  int ind = Population()/1000;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    ind += cur_tree->Industry(objs[ctr], oqty[ctr], this);
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

int Planet::SpareIndustry() {
  int ind = Industry();
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    ind -= cur_tree->Upkeep(objs[ctr], oqty[ctr], this);
    }
  return 0 >? ind;
  }

int Planet::Population() {
  return population;
  }

int Planet::PopulationM() {
  return pop_minor;
  }

int Planet::FreePop() {
  int pop = population;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    pop -= cur_tree->Crew(objs[ctr], oqty[ctr], this);
    }
  return pop;
  }

int Planet::FreePopM() {
  if(pop_minor == 0) return 0;
  else if(FreePop() < 0) return -1000000 + pop_minor;
  else return pop_minor;
  }

int Planet::Growth() {
  long long ret = population;
  ret *= Happiness();
  ret *= Security();
  ret /= 100000000;
  return ret;
  }

int Planet::GrowthM() {
  long long ret = population;
  ret *= Happiness();
  ret *= Security();
  ret %= 100000000;
  return ret;
  }

int Planet::Happiness() {
  int hap = FreePop()*100/Population();
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    hap += cur_tree->Happiness(objs[ctr], oqty[ctr], this);
    }
  return 0 >? hap <? 1000;
  }

int Planet::Security() {
  int sec = 0;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    sec += cur_tree->Security(objs[ctr], oqty[ctr], this);
    }
  return 0 >? sec <? 1000;
  }

int Planet::Loyalty() {
  int loy = 0;
  for(int ctr=0; ctr<(int)objs.size(); ++ctr) {
    loy += cur_tree->Loyalty(objs[ctr], oqty[ctr], this);
    }
  return loy;
  }

void Planet::TakeTurn() {
  if(claimed < 0) return;
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

  for(int ctr=0; ctr<num_satellites; ++ctr) satellites[ctr]->TakeTurn();

  if(SpareIndustry() > 0 && projs.size() > 0) {
    int indus = SpareIndustry(), need, ctr;
    Tech *tc;
    while(projs.size() > 0 && indus > 0) {
      tc = cur_tree->GetTech(projs[0]);
      need = tc->icost;
      if(prog[0]+indus >= need) {
	indus -= need-prog[0];
	for(ctr=0; ctr < (int)objs.size(); ++ctr) {
	  if(objs[ctr] == projs[0]) { ++oqty[ctr]; break; }
	  }
	if(ctr == (int)objs.size()) {
	  objs.push_back(projs[0]);
	  oqty.push_back(1);
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
