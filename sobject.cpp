#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "math.h"

#include "sobject.h"
#include "system.h"
#include "game.h"

//Orbital Period = sqrt(Distance From Star ^ 3)

SObject::SObject(System *s, int orb) {
  system = s;
  orbit = orb;
  period = int(sqrt(double(orbit)*double(orbit)*double(orbit)));
  startpos = rand()&65535;
  location = NULL;
  destination = NULL;
  depart_turn = 0;
  arrive_turn = 0;
  }

SObject::SObject(SObject *o) {
  system = o->Sys();
  orbit = o->orbit;
  period = o->period;
  startpos = o->startpos;
  location = o;
  destination = NULL;
  depart_turn = 0;
  arrive_turn = 0;
  }

void SObject::ComputeSPos(int turn) {
  if(location && destination) {
    int prog, trip = arrive_turn - depart_turn;
    if(trip < 1) trip = 1;
    if(turn >= arrive_turn) prog = trip;
    else if(turn <= depart_turn) prog = 0;
    else prog = turn - depart_turn;

    sxpos = location->SXPos(depart_turn) * (trip-prog);
    sxpos += destination->SXPos(arrive_turn) * prog;
    sxpos /= trip;
    sypos = location->SYPos(depart_turn) * (trip-prog);
    sypos += destination->SYPos(arrive_turn) * prog;
    sypos /= trip;
    }
  else {
    double ang = double(startpos)
	+ double(turn)*double(256*256*256) / double(period);
    double xp = double(orbit/20) * cos(ang * 2.0 * M_PIl / double(65536));
    double yp = double(orbit/20) * sin(ang * 2.0 * M_PIl / double(65536));
    sxpos = int(384+xp);
    sypos = int(384+yp);
    }

  if(location && abs(location->SXPos(turn) - sxpos) < 6
	&& abs(location->SYPos(turn) - sypos) < 6) {
    sxpos += 8; sypos -= 2;
    }

  vector<SObject*> present;
  for(int ctr=0; ctr < int(system->objects.size()); ++ctr)
	present.push_back(system->objects[ctr]);
  for(int ctr=0; ctr < int(system->fleets.size()); ++ctr)
	present.push_back(system->fleets[ctr]);
  int collide = 1;
  while(collide) {
    collide = 0; 
    for(int ctr=0; ctr < int(present.size()); ++ctr) {
      if(present[ctr] == this) break;
      if(present[ctr]->frame == cur_game->frame
		&& abs(present[ctr]->sxpos - sxpos) < 6
		&& abs(present[ctr]->sypos - sypos) < 6) {
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

  vector<SObject*> present;
  for(int ctr=0; ctr < int(system->objects.size()); ++ctr)
	present.push_back(system->objects[ctr]);
  for(int ctr=0; ctr < int(system->fleets.size()); ++ctr)
	present.push_back(system->fleets[ctr]);
  int collide = 1;
  while(collide) {
    collide = 0; 
    for(int ctr=0; ctr < int(present.size()); ++ctr) {
      if(present[ctr] == this) break;
      if(present[ctr]->frame == cur_game->frame
		&& abs(present[ctr]->gxpos - gxpos) < 6
		&& abs(present[ctr]->gypos - gypos) < 6) {
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

int SObject::SType() {
  return SOBJECT_PLANET;
  }

void SObject::TakeTurn() {
  if(destination && cur_game->turn >= arrive_turn) Arrive();
  }

void SObject::Arrive() {
  system = destination->Sys();
  orbit = destination->orbit;
  period = destination->period;
  startpos = destination->startpos;
  location = destination;
  destination = NULL;

  ((Planet*)location)->Explore(Owner());
  }

int SObject::Owner() {
  return -1;
  }
