#include "planet.h"

class System {
public:
  System(int xp, int yp, int nump, int minl, int atmosl);
  ~System();
  Planet **planets;
  int num_planets;
  int xpos;
  int ypos;
  };
