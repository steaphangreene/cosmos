#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "sobject.h"
#include "position.h"
#include "system.h"
#include "game.h"
#include "gui.h"

extern int max_factions;

//Orbital Period = sqrt(Distance From Star ^ 3)

vector<int> SObject::size(SOBJECT_MAX, 4);
vector<int> SObject::space(SOBJECT_MAX, 6);
vector<int> SObject::sqoff(SOBJECT_MAX, 9);

void SObject::Init() {
  location = NULL;
  destination = NULL;
  depart_turn = 0;
  arrive_turn = 0;
  target = NULL;
  distance = 0;
  seen.resize(max_factions, 0);
  known.resize(max_factions, 0);
  gturn = -1;
  sturn = -1;
  }

SObject::SObject(System *s, int orb) {
  Init();
  system = s;
  orbit = orb;
  period = int(sqrt(double(orbit)*double(orbit)*double(orbit)));
  startpos = rand()&65535;
  }

SObject::SObject(int a) {
  Init();
  }

SObject::SObject(SObject *o) {
  Init();
  system = o->Sys();
  orbit = o->orbit;
  period = o->period;
  startpos = o->startpos;
  location = GetPosition(o);
  }

void SObject::ComputeSLoc(int turn) {
  sturn = turn;
  double ang = double(startpos)
	+ double(turn)*double(256*256*256) / double(period);
  sxloc = int(double(orbit) * cos(ang * 2.0 * M_PIl / double(65536)));
  syloc = int(double(orbit) * sin(ang * 2.0 * M_PIl / double(65536)));

  if(destination) {
    int prog, trip = arrive_turn - depart_turn;
    if(trip < 1) trip = 1;
    if(turn >= arrive_turn) prog = trip;
    else if(turn <= depart_turn) prog = 0;
    else prog = turn - depart_turn;

    sxloc *= (trip-prog);
    sxloc += destination->SXLoc(arrive_turn) * prog;
    sxloc /= trip;
    syloc *= (trip-prog);
    syloc += destination->SYLoc(arrive_turn) * prog;
    syloc /= trip;
    }
  }

int SObject::SXLoc(int turn) {
  if(turn == sturn) return sxloc;
  ComputeSLoc(turn);
  return sxloc;
  }

int SObject::SYLoc(int turn) {
  if(turn == sturn) return syloc;
  ComputeSLoc(turn);
  return syloc;
  }

void SObject::ComputeSPos() {
  ComputeSLoc(cur_game->turn);
  frame = cur_game->frame;
  sxpos = (sxloc+10)/20 + 384;
  sypos = (syloc+10)/20 + 384;

  if(location && location->Represents()) {
    int space = (Space() + location->Represents()->Space()) / 2;
    if(abs(location->SXPos() - sxpos) < space
	&& abs(location->SYPos() - sypos) < space) {
      sxpos += space;
      sypos -= location->Represents()->Size()/2;
      sypos += Size()/2;
      }
    }

  int collide = 1;
  while(collide) {
    collide = 0; 
    for(int ctr=0; ctr < int(system->objects.size()); ++ctr) {
      if(system->objects[ctr] == this) break;
      int space = (Space() + system->objects[ctr]->Space()) / 2;
      if(system->objects[ctr]->frame == cur_game->frame
		&& abs(system->objects[ctr]->sxpos - sxpos) < space
		&& abs(system->objects[ctr]->sypos - sypos) < space) {
	sypos += space;
	collide = 1;
	break;
	}
      }
    }
  }

int SObject::SXPos() {
  if(frame == cur_game->frame) return sxpos;
  ComputeSPos();
  return sxpos;
  }

int SObject::SYPos() {
  if(frame == cur_game->frame) return sypos;
  ComputeSPos();
  return sypos;
  }

void SObject::ComputeGLoc(int turn) {
  gturn = turn;
  gxloc = system->GXLoc(turn);
  gyloc = system->GYLoc(turn);

  if(destination && destination->system != system) {
    int prog, trip = arrive_turn - depart_turn;
    if(trip < 1) trip = 1;
    if(turn >= arrive_turn) prog = trip;
    else if(turn <= depart_turn) prog = 0;
    else prog = turn - depart_turn;

    gxloc *= (trip-prog);
    gxloc += destination->GXLoc(arrive_turn) * prog;
    gxloc /= trip;
    gyloc *= (trip-prog);
    gyloc += destination->GYLoc(arrive_turn) * prog;
    gyloc /= trip;
    }
  }

int SObject::GXLoc(int turn) {
  if(turn == gturn) return gxloc;
  ComputeGLoc(turn);
  return gxloc;
  }

int SObject::GYLoc(int turn) {
  if(turn == gturn) return gyloc;
  ComputeGLoc(turn);
  return gyloc;
  }

void SObject::ComputeGPos() {
  ComputeGLoc(cur_game->turn);
  frame = cur_game->frame;
  gxpos = (gxloc+10)/20;
  gypos = (gyloc+10)/20;

  if(system && system != this) {
    int space = (Space() + system->Space()) / 2;
    if(abs(system->GXPos() - gxpos) < space
	&& abs(system->GYPos() - gypos) < space) {
      gxpos += space;
      gypos -= system->Size()/2;
      gypos += Size()/2;
      }
    }

  int collide = 1;
  while(collide) {
    collide = 0; 
    for(int ctr=0; ctr < int(system->objects.size()); ++ctr) {
      if(system->objects[ctr] == this) break;
      int space = (Space() + system->objects[ctr]->Space()) / 2;
      if(system->objects[ctr]->frame == cur_game->frame
		&& abs(system->objects[ctr]->gxpos - gxpos) < space
		&& abs(system->objects[ctr]->gypos - gypos) < space) {
	gypos += space;
	collide = 1;
	break;
	}
      }
    }




  }

int SObject::GXPos() {
  if(frame == cur_game->frame) return gxpos;
  ComputeGPos();
  return gxpos;
  }

int SObject::GYPos() {
  if(frame == cur_game->frame) return gypos;
  ComputeGPos();
  return gypos;
  }

SObject::~SObject() {
  }

void SObject::TakeTurn() {
  target = NULL;
  if(destination && cur_game->turn >= arrive_turn) Arrive();
  }

//FIXME - HAXXOR
void panel_draw();  // From gui.cpp
extern SObject *cur_object;  // From gui.cpp

void SObject::Arrive() {
  SObject *truedest = destination->Represents();
  if(!truedest) {
    system = destination->Sys();
    orbit = destination->OrbitDist();
    period = destination->Period();
    startpos = destination->OrbitPhase();
    arrive_turn = destination->ArriveTurn();
    depart_turn = destination->DepartTurn();
    Trash(location);
    location = destination->Location();
    Trash(destination);
    destination = destination->Destination();
    Trash(target);
    target = NULL;
    distance = 0;
    }
  else if(truedest->SType() == SOBJECT_PLANET) {
    system = destination->system;
    orbit = destination->orbit;
    period = destination->period;
    startpos = destination->startpos;
    Trash(location);
    location = destination;
    truedest->See(Owner());
    truedest->Know(Owner());
    destination = NULL;
    }
  else if(truedest->SType() == SOBJECT_SYSTEM) {
    orbit = 6600;
    period = int(sqrt(double(orbit)*double(orbit)*double(orbit)));

    int xp = system->gxpos - destination->system->gxpos;
    int yp = system->gypos - destination->system->gypos;
    double tang = double(cur_game->turn) * double(256*256*256) / double(period);
    double nang = double(65536) * atan2(double(yp), double(xp)) / 2.0 / M_PIl;
    startpos = int(nang - tang);

    system->FleetLeaves((Fleet*)this);
    system = destination->system;
    system->FleetArrives((Fleet*)this);

    Trash(location);
    location = NULL;
    truedest->Know(Owner());
    Trash(destination);
    destination = NULL;
    }
  else if(truedest->SType() == SOBJECT_FLEET && SType() == SOBJECT_FLEET
	&& Owner() == truedest->Owner()) {
    Fleet *flt = (Fleet *)this;
    for(int shp=0; shp < flt->NumShips(); ++shp)
      ((Fleet*)truedest)->AddShip(flt->GetShip(shp));
    if(cur_object == this) cur_object = truedest;
    panel_draw();
    RemapPositions(flt, truedest);
    flt->RemoveShips(0);
    }
  else if(truedest->SType() == SOBJECT_FLEET && SType() == SOBJECT_FLEET) {
    Fleet *flt = (Fleet*)this;
    flt->Attack((Fleet*)truedest);
    system = destination->system;
    orbit = destination->orbit;
    period = destination->period;
    startpos = destination->startpos;
    Trash(location);
    location = destination->location;
    Trash(destination);
    destination = destination->destination;  //FIXME! Two attacking each other!
    }
  else if(truedest->SType() == SOBJECT_FLEET) {
    system = destination->system;
    orbit = destination->orbit;
    period = destination->period;
    startpos = destination->startpos;
    Trash(location);
    location = destination->location;
    Trash(destination);
    destination = destination->destination;
    }
  }

int SObject::Owner() {
  return -1;
  }

int SObject::TimeToLocal(int sqdist) {
  if(SMove() <= 0) return INT_MIN;
  int ret = int(sqrt(double(sqdist)));
  ret += SMove()-1;
  ret /= SMove();
  return ret;
  }

int SObject::TimeToGalactic(int sqdist) {
  if(GMove() <= 0) return INT_MIN;

  //FIXME - Use Physics' Hyperspace Factor, etc...
  //int ret = int(sqrt(double(sqdist))*double(1000000));
  int ret = int(sqrt(double(sqdist)));

  ret += GMove()-1;
  ret /= GMove();
  return ret;
  }

void SObject::Engage() {
  if(target == NULL) return;
  Trash(destination);
  destination = target;
  depart_turn = cur_game->turn;
  arrive_turn = cur_game->turn + distance;
  if(arrive_turn == depart_turn) Arrive();
  target = NULL;
  }

void SObject::SetCourse(SObject *p) {
  if(!p) { target = NULL; return; }
  target = GetPosition(p);

  int tm = 1, offx, offy, sqd;

  if(target->system != system) {
    offx = abs(system->GXLoc(cur_game->turn)
	- target->system->GXLoc(cur_game->turn));
    offy = abs(system->GYLoc(cur_game->turn)
	- target->system->GYLoc(cur_game->turn));
    sqd = offx*offx + offy*offy;

    distance = TimeToGalactic(sqd);
    return;
    }

  if(destination && depart_turn < cur_game->turn) {
    SetOrigin();
    }

  for(int trip=0; ; ++trip) {
    offx = abs(target->SXLoc(cur_game->turn + trip)
	- SXLoc(cur_game->turn));
    offy = abs(target->SYLoc(cur_game->turn + trip)
	- SYLoc(cur_game->turn));
    sqd = offx*offx + offy*offy;

    tm = TimeToLocal(sqd);
    if(abs(tm) <= trip) {
      distance = (tm == 0) ? 0 : (tm/abs(tm))*trip;
      break;
      }
    }
  }

void SObject::SetOrigin() {
  int xp = (SXLoc(cur_game->turn));
  int yp = (SYLoc(cur_game->turn));
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
