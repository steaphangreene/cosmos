#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <algorithm>

using namespace std;

#include <SDL.h>

#include "gui.h"
#include "game.h"
#include "audio.h"
#include "fonts.h"
#include "graphics.h"
#include "gui_local.h"

extern unsigned long black;

extern SDL_Surface *screen;
extern SDL_Rect mouser;
extern SDL_Surface *cursor;

static SDL_Surface *gstar;

extern int buttlist[PAGE_MAX][BUTTON_MAX];
extern int pagemap[PAGE_MAX][BUTTON_MAX];

extern int page;
extern int cur_galaxy;
extern int cur_system;
extern int cur_planet;

void cursor_draw();

void gui_init_galaxy() {
  gstar = get_gstar_image();

  buttlist[PAGE_GALAXY][BUTTON_TURN] =		10;
  buttlist[PAGE_GALAXY][BUTTON_EXIT] =		11;
  pagemap[PAGE_GALAXY][BUTTON_EXIT] =		PAGE_ROOT;
//  ambient[PAGE_GALAXY] = audio_loadsound("sounds/ambient00.wav");
  }

//void gui_button_clicked_galaxy(int button) {
//  if(button == BUTTON_TURN) {
//    cur_game->TakeTurn();
//    }
//  }

void page_redraw_galaxy(SDL_Rect *area) {
//  MUST NOT CALL "page_update" - MOUSE CURSOR LOOP!!!!
/*
  SDL_Rect todo = *area;

  SDL_FillRect(screen, &todo, black);
  todo = *area;
  SDL_Rect rec = {0, 0, 3, 3};
  for(int snum=0; snum < cur_game->galaxys[cur_galaxy]->num_systems; ++snum) {
    System *sys = cur_game->galaxys[cur_galaxy]->systems[snum];
    int must = 0;
    if(sys->claimed > -1) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = sys->xpos - 4;
      rec2.y = sys->ypos - 4;
      if(overlaps(rec2, todo)) {
	SDL_FillRect(screen, &rec2, color3(
	  cur_game->players[sys->claimed]->color
	  ));
	must = 1;
	}
      }
    rec.x = sys->xpos - 1;
    rec.y = sys->ypos - 1;
    if(must || overlaps(rec, todo)) {
      SDL_BlitSurface(gstar, NULL, screen, &rec);
      }
    }
*/
  }

void page_init_galaxy() {
  }

void page_cleanup_galaxy() {
  }

void page_draw_galaxy() {
  SDL_Rect rec = {0, 0, 3, 3};
  for(int snum=0; snum < cur_game->galaxys[cur_galaxy]->num_systems; ++snum) {
    System *sys = cur_game->galaxys[cur_galaxy]->systems[snum];
    if(sys->claimed > -1) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = sys->xpos - 4;
      rec2.y = sys->ypos - 4;
      SDL_FillRect(screen, &rec2, color3(
	cur_game->players[sys->claimed]->color
	));
      }
    rec.x = sys->xpos - 1;
    rec.y = sys->ypos - 1;
    SDL_BlitSurface(gstar, NULL, screen, &rec);

    vector<int> present;
    for(int shp = 0; shp < int(sys->ships.size()); ++shp) {
      present.push_back(sys->ships[shp]->Owner());
      }
    for(int plan=0; plan < sys->num_planets; ++plan) {
      for(int shp = 0; shp < int(sys->planets[plan]->ships.size()); ++shp) {
	present.push_back(sys->planets[plan]->ships[shp]->Owner());
	}
      }
    if(present.size() > 0) {
//      sort(present.begin(), present.end());
//      present.erase(unique(present.begin(), present.end()), present.end());
      for(int ctr=0; ctr<int(present.size()); ++ctr) {
	SDL_Rect rec2 = {0, 0, 4, 4};
	rec2.x = sys->xpos + 6;
	rec2.y = sys->ypos - 4 + 6*ctr;
	SDL_FillRect(screen, &rec2,
		color3(cur_game->players[present[ctr]]->color));
	}
      }
    }
  }

void page_update_galaxy() {
  }

void page_clicked_galaxy(int mx, int my, int mb) {
  int offx, offy;
  for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
    offx = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - mx);
    offy = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - my);
    if(offx*offx + offy*offy <= 36) {
      cur_system = sys;
      page = PAGE_SYSTEM;
      break;
      }
    }
  }

void mouse_released_galaxy() {
  }

void mouse_moved_galaxy(int xp, int yp) {
  }
