class Satellite {
public:
  Satellite(int ang);
  ~Satellite();
  int XPos(int);
  int YPos(int);
  int InFront(int);

private:
  int startpos, angle;
  };
