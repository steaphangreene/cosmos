#ifndef SOBJECT_H
#define SOBJECT_H

#define SOBJECT_NONE	0
#define SOBJECT_PLANET	1
#define SOBJECT_FLEET	2

class System;

class SObject {
public:
  SObject(System *s, int orb);
  virtual ~SObject();
  virtual int SType();

  virtual void TakeTurn();

  System *Sys() { return system; };
  int OnFrame() { return frame; }

  void SetPos(int x, int y, int f=0) { xpos = x; ypos = y; frame = f; };
  int SXPos(int);
  int SYPos(int);

protected:
  System *system;
  int orbit, startturn, startpos, period;
  int frame, xpos, ypos;
  };

#endif
