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
extern font *cur_font[9];
extern SDL_Rect mouser;
extern SDL_Surface *cursor;

static SDL_Surface *splanet[4], *star;

extern int buttlist[PAGE_MAX][BUTTON_MAX];
extern int pagemap[PAGE_MAX][BUTTON_MAX];

extern int page;
extern int cur_galaxy;
extern int cur_system;
extern int cur_planet;

void cursor_draw();

void gui_init_system() {
  star = get_star_image();
  splanet[0] = get_splanet_image();

  buttlist[PAGE_SYSTEM][BUTTON_EXIT] =		11;
  pagemap[PAGE_SYSTEM][BUTTON_EXIT] =		PAGE_GALAXY;
  }

void page_redraw_system(SDL_Rect *area) {
/*
  SDL_Rect todo = *area;
  SDL_FillRect(screen, &todo, black);
  SDL_Rect destr = {(768-32)/2, (768-32)/2, 32, 32};
  if(overlaps(destr, todo)) SDL_BlitSurface(star, NULL, screen, &destr);
  destr.w = 3;  destr.h = 3;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < sys->num_planets; ++plan) {
    int must = 0;
    if(sys->planets[plan]->claimed >= 0) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = sys->planets[plan]->XPos(cur_game->turn) - 4;
      rec2.y = sys->planets[plan]->YPos(cur_game->turn) - 4;
      if(overlaps(rec2, todo)) {
	SDL_FillRect(screen, &rec2, color3(cur_game->players[
		sys->planets[plan]->claimed]->color));
	must = 1;
	}
      }
    destr.x = sys->planets[plan]->XPos(cur_game->turn) - 1;
    destr.y = sys->planets[plan]->YPos(cur_game->turn) - 1;
    if(must || overlaps(destr, todo))
      SDL_BlitSurface(splanet[0], NULL, screen, &destr);
    }
  todo = *area;
*/
  }

void page_init_system() {
  }

void page_cleanup_system() {
  }

void page_draw_system() {
  SDL_Rect destr = {(768-32)/2, (768-32)/2, 32, 32};
  SDL_BlitSurface(star, NULL, screen, &destr);
  destr.w = 3;  destr.h = 3;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < sys->num_planets; ++plan) {
    if(sys->planets[plan]->claimed >= 0) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = sys->planets[plan]->XPos(cur_game->turn) - 4;
      rec2.y = sys->planets[plan]->YPos(cur_game->turn) - 4;
      SDL_FillRect(screen, &rec2, color3(cur_game->players[
		sys->planets[plan]->claimed]->color));
      }
    destr.x = sys->planets[plan]->XPos(cur_game->turn) - 1;
    destr.y = sys->planets[plan]->YPos(cur_game->turn) - 1;
    SDL_BlitSurface(splanet[0], NULL, screen, &destr);
    if(sys->planets[plan]->ships.size() > 0) {
      SDL_Rect rec2 = {0, 0, 4, 4};
      rec2.x = sys->planets[plan]->XPos(cur_game->turn) + 6;
      rec2.y = sys->planets[plan]->YPos(cur_game->turn) - 4;
      SDL_FillRect(screen, &rec2, color3(cur_game->players[
		sys->planets[plan]->claimed]->color));
      }
    }
  }

void page_update_system() {
  }

void page_clicked_system(int mx, int my, int mb) {
  int offx, offy;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < sys->num_planets; ++plan) {
    offx = abs(mx - sys->planets[plan]->XPos(cur_game->turn));
    offy = abs(my - sys->planets[plan]->YPos(cur_game->turn));
    if(offx*offx + offy*offy <= 36) {
      cur_planet = plan;
      page = PAGE_PLANET;
      break;
      }
    }
  }

void mouse_released_system() {
  }

void mouse_moved_system(int xp, int yp) {
  }
