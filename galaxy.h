#include <vector>

using namespace std;

#include "player.h"
#include "system.h"

class Galaxy {
public:
  Galaxy(int numsys, int minl, int atmosl, int *devl) {
    vector<Player *> mtvec;
    Galaxy(numsys, minl, atmosl, devl, mtvec);
    };
  Galaxy(int numsys, int minl, int atmosl, int *devl, vector<Player *> &pl);

  void TakeTurn();

//private:
  System **systems;
  int num_systems;
  };
