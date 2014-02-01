#include <cstdio>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

using namespace std;

#include "renderer/renderer.h"
#include "graphics_ship.h"

#define PI 3.14159265358979323846

extern int fullscreen_mode;

#define ICON_HEIGHT     ((GLdouble)0.3)
#define ICON_DEPTH      ((GLdouble)-6.0)

struct Ship {
  string design;
  float bx, by;
  int stage;
  float xp, yp, zp;
  };

int main(int argc, char **argv) {
  fullscreen_mode = 1;
  deque<Ship> ships;
  int xsize=1024;
  int ysize=768;

  if(argc >= 3) {
    int tmp;
    tmp = atoi(argv[1]);
    if(tmp > 100 && tmp < 100000) xsize = tmp;
    tmp = atoi(argv[2]);
    if(tmp > 100 && tmp < 100000) ysize = tmp;
    }

  if(!init_renderer(xsize, ysize)) {
    fprintf(stderr, "Renderer failed to init!\n");
    exit(1);
    }

  scene cur_scene;

  vector<string> comps;
  comps.push_back("o");
  comps.push_back("O");
  comps.push_back("O");
  comps.push_back("O");
  comps.push_back("O");
  comps.push_back("O");
  comps.push_back("O");
  comps.push_back("O");
  comps.push_back("O");
  comps.push_back("|");
  comps.push_back("|");
  comps.push_back("|");
  comps.push_back("|");
  comps.push_back("|");
  comps.push_back("|");
  comps.push_back("|");
  comps.push_back("I");
  comps.push_back("I");
//  comps.push_back("[");

  vector<string> rots;
  rots.push_back("0");
  rots.push_back("1");
  rots.push_back("2");
  rots.push_back("3");
  rots.push_back("4");
  rots.push_back("5");
//  rots.push_back("6");
//  rots.push_back("7");
//  rots.push_back("8");
//  rots.push_back("9");

  int user_quit = 0;
  int user_wait = 0;

/*								Coming at screen
  float shipview_matrix[16] = {
    0.0, 0.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0,
    0.0, -1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0
    };
*/

  while(!user_quit) {
    while(ships.size() > 0 && ships.front().stage >= 200) {
      ships.pop_front();
      }

    start_scene(&cur_scene, 0);

    deque<Ship>::iterator shp = ships.begin();
    for(; shp != ships.end(); ++shp) {
      if(!user_wait) ++shp->stage;

/*								Coming at screen
      float ang = 0, yax = 1.0, zax = 0.0;
      if(shp->stage <= 80) {
	shp->zp = -30.0 + (0.25 * shp->stage);
	shp->xp = shp->bx;
	shp->yp = shp->by;
	}
      else if(shp->stage >= 120) {
	ang = 180.0;
	shp->zp = -30.0 + (0.25 * (200 - shp->stage));
	if(fabsf(shp->bx) > fabsf(shp->by)) {
	  swap(yax, zax);
	  shp->xp = -shp->bx;
	  shp->yp = shp->by;
	  }
	else {
	  shp->xp = shp->bx;
	  shp->yp = -shp->by;
	  }
	}
      else {
	ang = 4.5 * (shp->stage - 80);
	float prop = cos(ang*PI/180.0);
	float zprop = sin(ang*PI/180.0);

	if(fabsf(shp->bx) > fabsf(shp->by)) {
	  swap(yax, zax);
	  if(shp->bx > 0.0) zax = -1.0;
	  shp->xp = shp->bx*prop;
	  shp->yp = shp->by;
	  shp->zp = -10.0 + fabsf(shp->bx)*zprop;
	  }
	else {
	  if(shp->by > 0.0) yax = -1.0;
	  shp->xp = shp->bx;
	  shp->yp = shp->by*prop;
	  shp->zp = -10.0 + fabsf(shp->by)*zprop;
	  }
	}
      glLoadIdentity();
      glTranslatef(shp->xp, shp->yp, shp->zp);
      glMultMatrixf(shipview_matrix);
      if(ang > 0.0) glRotatef(ang, 0.0, yax, zax);
      render_ship(shp->design);
*/

      float ang=0.0;

      shp->xp = float(shp->stage-100)/50.0;
      shp->yp = shp->by;
      shp->zp = -10.0;

      if(shp->stage >= 55 && shp->stage < 145) ang = float(shp->stage-55)*4.0;

      glLoadIdentity();
      glTranslatef(shp->xp, shp->yp, shp->zp);
      if(ang > 0.0) glRotatef(ang, 1.0, 0.0, 0.0);
      render_ship(shp->design);
      }

    finish_scene(&cur_scene, 0);

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch (event.type) {
	case(SDL_KEYDOWN): {
	  if(event.key.keysym.sym == SDLK_ESCAPE) {
	    user_quit = 1;
	    }
	  else if(event.key.keysym.sym == SDLK_SPACE) {
	    user_wait = !user_wait;
	    }
	  else if(event.key.keysym.sym == SDLK_MINUS) {
	    deque<Ship>::iterator shp = ships.begin();
	    for(; shp != ships.end(); ++shp) shp->stage -= 50;
	    }
	  }
	}
      }

//    if((!user_wait) && (rand()%100) < 5) {
//    if((!user_wait) && (rand()%100) < 10 && ships.size() < 3) {
    if((!user_wait) && (ships.size() <= 0 || ships.back().stage >= 40)) {
//      Ship aship = { "O51I21| 54|",
      Ship aship = { "",
	(float(rand()%1000) / 1000.0 + 0.5) * float(-1+2*(rand()%2)),
	(float(rand()%1000) / 1000.0 + 0.0) * float(-1+2*(rand()%2)),
	0, 0.0, 0.0, 0.0
	};

///*
      int complexity = 2+(rand()%3);
      for(int ctr=0; ctr<complexity; ++ctr) {
	if(ctr != 0) {
	  aship.design += rots[rand()%rots.size()/2];
	  aship.design += rots[rand()%rots.size()/2+rots.size()/2];
	  }
	aship.design += comps[rand()%comps.size()];
	}
//*/
      ships.push_back(aship);
      }
    }

  exit(0);
  return 0;
  }
