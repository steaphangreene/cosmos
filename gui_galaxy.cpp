#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

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

  buttlist[PAGE_GALAXY][BUTTON_EXIT] =		11;
  pagemap[PAGE_GALAXY][BUTTON_EXIT] =		PAGE_ROOT;
//  ambient[PAGE_GALAXY] = audio_loadsound("sounds/ambient00.wav");
  }

void gui_button_clicked_galaxy(int button) {
  }

void panel_redraw_galaxy(SDL_Rect *area) {
  }

void page_redraw_galaxy(SDL_Rect *area) {
//  MUST NOT CALL "page_update" - MOUSE CURSOR LOOP!!!!
  SDL_Rect todo = *area;

  SDL_FillRect(screen, &todo, black);
  todo = *area;
  SDL_Rect rec = {0, 0, 3, 3};
  for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
    int must = 0;
    if(cur_game->galaxys[cur_galaxy]->systems[sys]->claimed > -1) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - 4;
      rec2.y = cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - 4;
      if(overlaps(rec2, todo)) {
	SDL_FillRect(screen, &rec2, color3(
	  cur_game->players[
		cur_game->galaxys[cur_galaxy]->systems[sys]->claimed]->color
	  ));
	must = 1;
	}
      }
    rec.x = cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - 1;
    rec.y = cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - 1;
    if(must || overlaps(rec, todo)) {
      SDL_BlitSurface(gstar, NULL, screen, &rec);
      }
    }
  }

void page_draw_galaxy() {
  SDL_Rect rec = {0, 0, 3, 3};
  for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
    if(cur_game->galaxys[cur_galaxy]->systems[sys]->claimed > -1) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - 4;
      rec2.y = cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - 4;
      SDL_FillRect(screen, &rec2, color3(
	cur_game->players[cur_game->galaxys[cur_galaxy]->systems[sys]->claimed]
	->color
	));
      }
    rec.x = cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - 1;
    rec.y = cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - 1;
    SDL_BlitSurface(gstar, NULL, screen, &rec);
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
