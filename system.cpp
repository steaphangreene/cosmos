#include <cstdio>
#include <cstdlib>

#include "math.h"

#include "system.h"

System::System(int xp, int yp, int nump, int minl, int atmosl, int *devl, int pl) {
  claimed = pl;
  num_planets = nump;
  planets = new (Planet*)[nump];
  for(int ctr=0; ctr<nump; ++ctr) {
    if(pl >= 0 && ctr == 2) {
      planets[ctr] = new Planet(ctr, (rand()%10000)+1, 30, 30);
      planets[ctr]->claimed = pl;
      planets[ctr]->population = 500;
      for(int ctr2=0; ctr2<cur_tree->NumTechs(); ++ctr2) {
	Tech *tc = cur_tree->GetTech(ctr2);
	if(tc->type == TECH_STRUCTURE && tc->known[pl]) {
	  planets[ctr]->objs.push_back(ctr2);
	  planets[ctr]->oqty.push_back(devl[ctr2]);
	  planets[ctr]->population += tc->crew * devl[ctr2];
	  }
	}
      for(int ctr2=0; ctr2<cur_tree->NumTechs(); ++ctr2) {
	Tech *tc = cur_tree->GetTech(ctr2);
	if(tc->type == TECH_PROJECT && tc->known[pl]) {
	  planets[ctr]->objs.push_back(ctr2);
	  planets[ctr]->oqty.push_back(devl[ctr2]);
	  planets[ctr]->population += tc->crew * devl[ctr2];
	  }
	}
      for(int ctr2=0; ctr2<cur_tree->NumTechs(); ++ctr2) {
	Tech *tc = cur_tree->GetTech(ctr2);
	if(tc->type == TECH_SHIP) {
	  for(int shp=0	; shp < devl[ctr2]; ++shp) {
	    planets[ctr]->ships.push_back(new Ship(tc->special, pl));
	    (*(planets[ctr]->ships.end()-1))->AddCrew(
		(*(planets[ctr]->ships.end()-1))->MaxCrew());
	    }
	  }
	}
      }
    else {
      int at=0, mi=0;
      if(atmosl == 3) at = int(double(sqrt(double(rand()%901))));
      else if(atmosl == 2) at = rand()%30;
      else if(atmosl == 1) at = (rand()%30)*(rand()%30)/30;
      if(minl == 3) mi = int(double(sqrt(double(rand()%901))));
      else if(minl == 2) mi = rand()%30;
      else if(minl == 1) mi = (rand()%30)*(rand()%30)/30;
      planets[ctr] = new Planet(ctr, (rand()%10000)+1, mi, at);
      }
    }
  xpos = xp;
  ypos = yp;
  }

System::~System() {
  for(int ctr=0; ctr < num_planets; ++ctr) {
    delete planets[ctr];
    }
  delete [] planets;
  planets = NULL;
  }

void System::TakeTurn() {
  for(int ctr=0; ctr<num_planets; ++ctr) planets[ctr]->TakeTurn();
  }
