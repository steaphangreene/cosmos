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

static SDL_Surface *splanet[4], *star;

void gui_init_system() {
  star = get_star_image();
  splanet[0] = get_splanet_image();

  pagemap[PAGE_SYSTEM][BUTTON_EXIT] =		PAGE_GALAXY;
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
  ++cur_game->frame;
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
    for(int ctr=0; ctr < int(sys->planets[plan]->fleets.size()); ++ctr) {
      SDL_Rect rec2 = {0, 0, 4, 4};
      sys->planets[plan]->fleets[ctr]->SetPos(
	sys->planets[plan]->XPos(cur_game->turn) + 8,
	sys->planets[plan]->YPos(cur_game->turn) + 6*ctr - 2,
	cur_game->frame
	);
      rec2.x = sys->planets[plan]->fleets[ctr]->XPos() - 2;
      rec2.y = sys->planets[plan]->fleets[ctr]->YPos() - 2;
      SDL_FillRect(screen, &rec2, color3(cur_game->players[
		sys->planets[plan]->claimed]->color));
      }
    }
  }

void page_update_system() {
  }

void page_clicked_system(int mx, int my, int mb) {
  int offx, offy, fr;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < sys->num_planets; ++plan) {
    offx = abs(sys->planets[plan]->XPos(cur_game->turn) - mx);
    offy = abs(sys->planets[plan]->YPos(cur_game->turn) - my);
    if(offx*offx + offy*offy <= 36) {
      cur_planet = plan;
      page = PAGE_PLANET;
      break;
      }
    }
  for(int flt=0; flt < int(cur_game->fleets.size()); ++flt) {
    offx = abs(cur_game->fleets[flt]->XPos() - mx);
    offy = abs(cur_game->fleets[flt]->YPos() - my);
    fr = cur_game->fleets[flt]->OnFrame();
    if(cur_game->frame == fr && offx*offx + offy*offy <= 9) {
      cur_fleet = flt;
      panel = PANEL_FLEET;
      panel_init();
      return;
      }
    }
  }

void mouse_released_system() {
  }

void mouse_moved_system(int xp, int yp) {
  }
