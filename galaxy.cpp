#include <cstdio>
#include <cstdlib>

#include "galaxy.h"

Galaxy::Galaxy(int numsys, int minl, int atmosl) {
  num_systems = numsys;
  systems = new (System*)[numsys];
  for(int ctr=0; ctr<numsys; ++ctr) {
    systems[ctr] = new System(34+rand()%700, 34+rand()%700,
	rand()%10, minl, atmosl);
    }
  }


