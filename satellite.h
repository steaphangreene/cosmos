class Satellite {
public:
  Satellite(int ang, int tp);
  ~Satellite();
  int XPos(int);
  int YPos(int);
  int Type() { return type; }
  int InFront(int);

  void TakeTurn();

private:
  int startpos, angle, type;
  };
