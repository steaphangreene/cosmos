#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "sobject.h"

//Orbital Period = sqrt(Distance From Star ^ 3)

SObject::SObject(System *s, int orb) {
  system = s;
  orbit = orb;
  period = int(sqrt(double(orbit)*double(orbit)*double(orbit)));
  startpos = rand()&65535;
  startturn = 0;
  }

int SObject::SXPos(int turn) {
  turn -= startturn;
  if(orbit) {
    double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
    double xpos = double(orbit/20) * cos(ang * 2.0 * M_PIl / double(65536));
    return int(384+xpos);
    }
  else {
    return xpos;
    }
  }

int SObject::SYPos(int turn) {
  turn -= startturn;
  if(orbit) {
    double ang = double(startpos) + double(turn)*double(256*256*256) / double(period);
    double ypos = double(orbit/20) * sin(ang * 2.0 * M_PIl / double(65536));
    return int(384+ypos);
    }
  else {
    return ypos;
    }
  }

SObject::~SObject() {
  }

int SObject::SType() {
  return SOBJECT_PLANET;
  }

void SObject::TakeTurn() {
  fprintf(stderr, "Warning: Bad SObject Taking Turn!\n");
  }
