#include <vector>
#include <string>

using namespace std;

#ifndef SOBJECT_H
#define SOBJECT_H

enum {
	SOBJECT_NONE=0,
	SOBJECT_PLANET,
	SOBJECT_FLEET,
	SOBJECT_MAX
	};

class System;

class SObject {
public:
  SObject(SObject *o);
  SObject(System *s, int orb);
  virtual ~SObject();
  virtual int SType() { return SOBJECT_NONE; }

  virtual void TakeTurn();

  const char *Name() { return name.c_str(); };
  void SetName(const char *nm) { name = nm; };

  System *Sys() { return system; };
  int OnFrame() { return frame; };

  virtual int Owner();

  SObject *Location() { return location; };
  SObject *Destination() { return destination; };
  int ArriveTurn() { return arrive_turn; }
  int DepartTurn() { return depart_turn; }
  void Arrive();

  SObject *Target() { return target; };
  int Distance() { return distance; };
  void SetCourse(SObject *);
  void Engage();

  int Period() { return period; }
  int OrbitDist() { return orbit; }

  void ComputeSPos(int);
  int SXPos(int);
  int SYPos(int);
  void ComputeGPos(int);
  int GXPos(int);
  int GYPos(int);

  int SeenBy(int);
  void See(int);
  int KnownTo(int);
  void Know(int);

protected:
  string name;
  System *system;
  int orbit, startpos, period;
  int frame, sxpos, sypos, gxpos, gypos;
  SObject *location, *destination, *target;
  int depart_turn, arrive_turn, distance;

  void SetOrigin();
  int TimeToLocal(int sqdst);
  int TimeToGalactic(int sqdst);

  vector<int> seen;
  vector<int> known;
  };

#endif

