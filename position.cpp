#include <cstdio> // FIXME - NOT NEEDED

#include "game.h"
#include "position.h"

Position::Position() : SObject(1) {
  next = NULL;
  }

Position::~Position() {
  }

void Position::TakeTurn() {
  }

const char *Position::Name() {
  if(represents) return represents->Name();
  return "Target Location";
  };

void Position::AssignTo(SObject *o) {
  system = o->Sys();
  orbit = o->OrbitDist();
  period = o->Period();
  startpos = o->OrbitPhase();
  location = o->Location();
  destination = o->Destination();
  arrive_turn = o->ArriveTurn();
  depart_turn = o->DepartTurn();
  target = NULL;
  distance = 0;
  represents = o;
  }

void Position::ComputeSPos() {
  frame = cur_game->frame;
  if(represents) {
    sxpos = represents->SXPos();
    sypos = represents->SYPos();
    }
  else {
    ComputeSLoc(cur_game->turn);
    sxpos = (sxloc+10)/20 + 384;
    sypos = (syloc+10)/20 + 384;
    }
  }

void Position::ComputeGPos() {
  frame = cur_game->frame;
  if(represents) {
    gxpos = represents->GXPos();
    gypos = represents->GYPos();
    }
  else {
    ComputeGLoc(cur_game->turn);
    gxpos = (gxloc+10)/20;
    gypos = (gyloc+10)/20;
    }
  }

#define POS_BLOCKSIZE 1024
static Position *unused_pos = NULL;
static Position *used_pos = NULL;

Position *GetPosition(SObject *s) {
  Position *ret;

  if(!unused_pos) {
    int ctr;
    //printf("Creating new Position Block.\n");
    unused_pos = new Position[POS_BLOCKSIZE];
    for(ctr=0; ctr < (POS_BLOCKSIZE - 1); ++ctr) {
      unused_pos[ctr].next = &unused_pos[ctr+1];
      }
    unused_pos[ctr].next = NULL;
    }

  ret = unused_pos;
  unused_pos = unused_pos->next;
  ret->next = used_pos;
  used_pos = ret;

  ret->AssignTo(s);

  return ret;
  }

void RecyclePosition(Position *p) {
  if(!p) return;

  Position **cur = &used_pos;
  while(*cur) {
    if((*cur) == p) {
      *cur = (*cur)->next;
      break;
      }
    cur = &((*cur)->next);
    }

  p->next = unused_pos;
  unused_pos = p;
  }

void RemapPositions(SObject *oldo, SObject *newo) {
  for(Position *pos = used_pos; pos; pos = pos->next) {
    if(pos->represents == oldo) pos->represents = newo;
    }
  }

void CleanPositions(vector <SObject*> &objs) {
  for(int obj = 0; obj < int(objs.size()); ++obj) {
    RemapPositions(objs[obj], NULL);
    }
  }
