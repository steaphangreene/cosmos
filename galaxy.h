#include "player.h"
#include "system.h"

class Galaxy {
public:
  Galaxy(int numsys, int minl, int atmosl, int devl, int nump=0, Player **pl=NULL);

  void TakeTurn();

//private:
  System **systems;
  int num_systems;
  };
