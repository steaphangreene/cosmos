#include <cstdio>
#include <cstdlib>

#include "math.h"

#include "satellite.h"

Satellite::Satellite(int ang) {
  startpos = rand()&65535;
  angle = ang;
  }

Satellite::~Satellite() {
  }

int Satellite::XPos(int tick) {
  int pos = startpos;
  pos += tick*256;
  pos &= 65535;
  double loc = 380.0 * cos(pos * 2.0 * M_PIl / double(65536));
  return 384+int(loc * cos(angle * 2.0 * M_PIl / double(65536)));
  }

int Satellite::YPos(int tick) {
  int pos = startpos;
  pos += tick*256;
  pos &= 65535;
  double loc = 380.0 * cos(pos * 2.0 * M_PIl / double(65536));
  return 384+int(loc * sin(angle * 2.0 * M_PIl / double(65536)));
  }

int Satellite::InFront(int tick) {
  int pos = startpos;
  pos += tick*256;
  pos &= 65535;
  if(pos&32768) return 1;
  return 0;
  }
