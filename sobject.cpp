#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "sobject.h"
#include "system.h"
#include "game.h"

extern int max_factions;

//Orbital Period = sqrt(Distance From Star ^ 3)

SObject::SObject(System *s, int orb) {
  name = "Stellar Object";
  system = s;
  orbit = orb;
  period = int(sqrt(double(orbit)*double(orbit)*double(orbit)));
  startpos = rand()&65535;
  location = NULL;
  destination = NULL;
  depart_turn = 0;
  arrive_turn = 0;
  target = NULL;
  distance = 0;
  seen.resize(max_factions, 0);
  known.resize(max_factions, 0);
  }

SObject::SObject(SObject *o) {
  name = "Stellar Object";
  system = o->Sys();
  orbit = o->orbit;
  period = o->period;
  startpos = o->startpos;
  location = o;
  destination = NULL;
  depart_turn = 0;
  arrive_turn = 0;
  target = NULL;
  distance = 0;
  seen.resize(max_factions, 0);
  known.resize(max_factions, 0);
  }

void SObject::ComputeSPos(int turn) {
  double ang = double(startpos)
	+ double(turn)*double(256*256*256) / double(period);
  double xp = double(orbit/20) * cos(ang * 2.0 * M_PIl / double(65536));
  double yp = double(orbit/20) * sin(ang * 2.0 * M_PIl / double(65536));
  sxpos = int(384+xp);
  sypos = int(384+yp);

  if(location && destination) {
    int prog, trip = arrive_turn - depart_turn;
    if(trip < 1) trip = 1;
    if(turn >= arrive_turn) prog = trip;
    else if(turn <= depart_turn) prog = 0;
    else prog = turn - depart_turn;

    sxpos *= (trip-prog);
    sxpos += destination->SXPos(arrive_turn) * prog;
    sxpos /= trip;
    sypos *= (trip-prog);
    sypos += destination->SYPos(arrive_turn) * prog;
    sypos /= trip;
    }

  if(location && abs(location->SXPos(turn) - sxpos) < 6
	&& abs(location->SYPos(turn) - sypos) < 6) {
    sxpos += 8; sypos -= 2;
    }

  int collide = 1;
  while(collide) {
    collide = 0; 
    for(int ctr=0; ctr < int(system->objects.size()); ++ctr) {
      if(system->objects[ctr] == this) break;
      if(system->objects[ctr]->frame == cur_game->frame
		&& abs(system->objects[ctr]->sxpos - sxpos) < 6
		&& abs(system->objects[ctr]->sypos - sypos) < 6) {
	sypos += 6;
	collide = 1;
	break;
	}
      }
    }
  if(turn == cur_game->turn) frame = cur_game->frame;
  else frame = cur_game->frame - 1;
  }

int SObject::SXPos(int turn) {
  if(turn == cur_game->turn && frame == cur_game->frame) return sxpos;
  ComputeSPos(turn);
  return sxpos;
  }

int SObject::SYPos(int turn) {
  if(turn == cur_game->turn && frame == cur_game->frame) return sypos;
  ComputeSPos(turn);
  return sypos;
  }

void SObject::ComputeGPos(int turn) {
  gxpos = system->xpos + 8;
  gypos = system->ypos - 2;

  int collide = 1;
  while(collide) {
    collide = 0; 
    for(int ctr=0; ctr < int(system->objects.size()); ++ctr) {
      if(system->objects[ctr] == this) break;
      if(system->objects[ctr]->frame == cur_game->frame
		&& abs(system->objects[ctr]->gxpos - gxpos) < 6
		&& abs(system->objects[ctr]->gypos - gypos) < 6) {
	gypos += 6;
	collide = 1;
	break;
	}
      }
    }
  if(turn == cur_game->turn) frame = cur_game->frame;
  else frame = cur_game->frame - 1;
  }

int SObject::GXPos(int turn) {
  if(turn == cur_game->turn && frame == cur_game->frame) return gxpos;
  ComputeGPos(turn);
  return gxpos;
  }

int SObject::GYPos(int turn) {
  if(turn == cur_game->turn && frame == cur_game->frame) return gypos;
  ComputeGPos(turn);
  return gypos;
  }

SObject::~SObject() {
  }

void SObject::TakeTurn() {
  target = NULL;
  if(destination && cur_game->turn >= arrive_turn) Arrive();
  }

extern Fleet *cur_object;  // From gui.cpp
void SObject::Arrive() {
  if(destination->SType() == SOBJECT_PLANET) {
    system = destination->system;
    orbit = destination->orbit;
    period = destination->period;
    startpos = destination->startpos;
    location = destination;
    ((Planet*)location)->See(Owner());
    ((Planet*)location)->Know(Owner());
    destination = NULL;
    }
  else if(destination->SType() == SOBJECT_FLEET) {
    Fleet *flt = (Fleet *)this;
    vector<Ship*>::iterator shp = flt->ships.begin(); 
    for(; shp != flt->ships.end(); ++shp) {
      ((Fleet*)destination)->ships.push_back(*shp);
      }
    flt->ships.clear();
    if(cur_object == this) cur_object = (Fleet*)destination;
    cur_game->junk.push_back(this);
    }
  }

int SObject::Owner() {
  return -1;
  }

int SObject::TimeToLocal(int sqdist) {
  return (int(sqrt(double(sqdist)))+19)/20;
  }

int SObject::TimeToGalactic(int sqdist) {
  return TimeToLocal(sqdist)*1000000;
  }

void SObject::Engage() {
  if(target == NULL) return;
  destination = target;
  depart_turn = cur_game->turn;
  arrive_turn = cur_game->turn + distance;
  if(arrive_turn == depart_turn) Arrive();
  target = NULL;
  }

void SObject::SetCourse(SObject *p) {
  target = p;
  if(!target) return;

  if(destination && depart_turn < cur_game->turn) {
    SetOrigin();
    }

  int tm = 1, offx, offy, sqd;
  SObject *origin = this;
  if(location && ((!destination)
	|| depart_turn >= cur_game->turn))
    origin = location;
  SObject *goal = target;
  if(goal->location && ((!goal->destination)
	|| goal->depart_turn >= cur_game->turn))
    goal = goal->location;
  for(int trip=0; ; ++trip) {
    offx = abs(goal->SXPos(cur_game->turn + trip)
	- origin->SXPos(cur_game->turn));
    offy = abs(goal->SYPos(cur_game->turn + trip)
	- origin->SYPos(cur_game->turn));
    sqd = offx*offx + offy*offy;

    tm = TimeToLocal(sqd);
    if(abs(tm) <= trip) {
      distance = (tm == 0) ? 0 : (tm/abs(tm))*trip;
      break;
      }
    }
  }

void SObject::SetOrigin() {
  int xp = (SXPos(cur_game->turn)-384)*20;
  int yp = (SYPos(cur_game->turn)-384)*20;
  orbit = int(sqrt(double(xp*xp+yp*yp)));
  period = int(sqrt(double(orbit)*double(orbit)*double(orbit)));

  double tang = double(cur_game->turn) * double(256*256*256) / double(period);
  double nang = double(65536) * atan2(double(yp), double(xp)) / 2.0 / M_PIl;
  startpos = int(nang - tang);
  }

int SObject::SeenBy(int n) {
  return seen[n];
  }

void SObject::See(int n) {
  seen[n] = 1;
  }

int SObject::KnownTo(int n) {
  return known[n];
  }

void SObject::Know(int n) {
  known[n] = 1;
  }
