#include <vector>

using namespace std;

class Position;

#ifndef POSITION_H
#define POSITION_H

#include "sobject.h"

class Position : public SObject {
public:
  Position(SObject *);
  virtual ~Position();
  virtual void TakeTurn();
  virtual int SType() { return SOBJECT_POSITION; }
  SObject *Represents() { return represents; }
  virtual const char *Name();

protected:
  Position *next;
  SObject *represents;
  virtual void ComputeSPos();
  virtual void ComputeGPos();
  void AssignTo(SObject *);
  friend Position *GetPosition(SObject *);
  friend void RecyclePosition(Position *);
  friend void CleanPositions(vector<SObject*>&);
  friend void RemapPositions(SObject *oldo, SObject *newo);
  };

Position *GetPosition(SObject *);
void RecyclePosition(Position *);
void CleanPositions(vector<SObject*>&);
void RemapPositions(SObject *oldo, SObject *newo);

#endif
