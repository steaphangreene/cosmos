#include <vector>
#include <string>

using namespace std;

#ifndef SOBJECT_H
#define SOBJECT_H

enum {
	SOBJECT_NONE=0,
	SOBJECT_POSITION,
	SOBJECT_PLANET,
	SOBJECT_FLEET,
	SOBJECT_SYSTEM,
	SOBJECT_MAX
	};

class System;
class Position;

class SObject {
public:
  SObject(int); // Placeholder for Positions to use
  SObject(SObject *o);
  SObject(System *s, int orb = 0);
  virtual ~SObject();
  virtual int SType() { return SOBJECT_NONE; }
  virtual int SMove() { return 0; }
  virtual int GMove() { return 0; }

  virtual void TakeTurn();

  System *Sys() { return system; };
  int OnFrame() { return frame; };

  virtual int Owner();
  virtual const char *Name() { return "Stellar Object"; };
  virtual void SetName(const char *nm) { };

  Position *Location() { return location; };
  Position *Destination() { return destination; };
  int ArriveTurn() { return arrive_turn; }
  int DepartTurn() { return depart_turn; }
  void Arrive();

  Position *Target() { return target; };
  int Distance() { return distance; };
  void SetCourse(SObject *);
  void Engage();

  int Period() { return period; }
  int OrbitDist() { return orbit; }
  int OrbitPhase() { return startpos; }

  int SXPos();
  int SYPos();
  int GXPos();
  int GYPos();

  int SXLoc(int);
  int SYLoc(int);
  int GXLoc(int);
  int GYLoc(int);

  virtual int SeenBy(int);
  virtual void See(int);
  virtual int KnownTo(int);
  virtual void Know(int);

protected:
  void Init();

  System *system;
  int orbit, startpos, period;
  int frame, sxpos, sypos, gxpos, gypos;
  int sturn, sxloc, syloc;
  int gturn, gxloc, gyloc;
  Position *location, *destination, *target;
  int depart_turn, arrive_turn, distance;

  void SetOrigin();
  int TimeToLocal(int sqdst);
  int TimeToGalactic(int sqdst);

  virtual void ComputeSPos();
  virtual void ComputeGPos();
  void ComputeSLoc(int);
  void ComputeGLoc(int);

  vector<int> seen;
  vector<int> known;
  };

#endif

