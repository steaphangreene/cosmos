#include "satellite.h"

class Planet {
public:
  Planet(int ord, int sz, int min, int atmos);
  ~Planet();
  int XPos(int);
  int YPos(int);
  int order, size, minerals, atmosphere, type;
  int startpos, period;
  int num_satellites;
  Satellite **satellites;
  };
