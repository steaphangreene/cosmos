#include <cstdio>
#include <cstdlib>

#include "system.h"

System::System(int xp, int yp, int nump, int minl, int atmosl, int pl) {
  claimed = pl;
  num_planets = nump;
  planets = new (Planet*)[nump];
  for(int ctr=0; ctr<nump; ++ctr) {
    if(pl >= 0 && ctr == 2) {
      planets[ctr] = new Planet(ctr, (rand()%10000)+1, 10 * minl, 10 * atmosl);
      planets[ctr]->claimed = pl;
      }
    else {
      planets[ctr] = new Planet(ctr, (rand()%10000)+1,
	(rand()%10) * minl, (rand()%10) * atmosl);
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
