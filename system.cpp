#include <cstdio>
#include <cstdlib>

#include "system.h"

//Orbital Period = sqrt(Distance From Star ^ 3)

System::System(int xp, int yp, int nump, int minl, int atmosl) {
  num_planets = nump;
  planets = new (Planet*)[nump];
  for(int ctr=0; ctr<nump; ++ctr) {
    planets[ctr] = new Planet(ctr, (rand()%10000)+1,
	(rand()%10) * minl, (rand()%10) * atmosl);
    }
  xpos = xp;
  ypos = yp;
  }

