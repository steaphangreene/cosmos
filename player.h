#include <cstdio>

class Player {
public:
  Player();
  Player(FILE *);

  void TakeTurn();

  void SaveTo(FILE *);
  void LoadFrom(FILE *);

  int color;
  };

extern int local_player;
