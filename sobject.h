#ifndef SOBJECT_H
#define SOBJECT_H

#define SOBJECT_NONE	0
#define SOBJECT_PLANET	1
#define SOBJECT_FLEET	2

class System;

class SObject {
public:
  SObject(SObject *o);
  SObject(System *s, int orb);
  virtual ~SObject();
  virtual int SType();

  virtual void TakeTurn();

  System *Sys() { return system; };
  int OnFrame() { return frame; }

  virtual int Owner();

  SObject *Location() { return location; };
  SObject *Destination() { return destination; };
  void Arrive();

  void ComputeSPos(int);
  int SXPos(int);
  int SYPos(int);
  void ComputeGPos(int);
  int GXPos(int);
  int GYPos(int);

protected:
  System *system;
  int orbit, startpos, period;
  int frame, sxpos, sypos, gxpos, gypos;
  SObject *location, *destination;
  int depart_turn, arrive_turn;
  };

#endif
