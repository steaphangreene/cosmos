#include <deque>

using namespace std;

#include "planet.h"
#include "ship.h"

class System {
public:
  System(int xp, int yp, int nump, int minl, int atmosl, int *devl, int pl=-1);
  ~System();

  void TakeTurn();

//private:
  deque<Ship*> ships;
  Planet **planets;
  int num_planets;
  int xpos;
  int ypos;
  int claimed;
  };
