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

static SDL_Surface *gstar;

void gui_init_galaxy() {
  gstar = get_gstar_image();

  buttlist[PANEL_GAME][BUTTON_TURN] =		10;
  buttlist[PANEL_GAME][BUTTON_EXIT] =		11;
  pagemap[PAGE_GALAXY][BUTTON_EXIT] =		PAGE_ROOT;
//  ambient[PAGE_GALAXY] = audio_loadsound("sounds/ambient00.wav");
  }

//void gui_button_clicked_galaxy(int button) {
//  if(button == BUTTON_TURN) {
//    cur_game->TakeTurn();
//    }
//  }

void page_init_galaxy() {
  if(panel < PANEL_GAME) panel = PANEL_GAME;
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

    vector<Fleet *> present;
    for(int shp = 0; shp < int(sys->fleets.size()); ++shp) {
      present.push_back(sys->fleets[shp]);
      }
    for(int plan=0; plan < sys->num_planets; ++plan) {
      for(int shp = 0; shp < int(sys->planets[plan]->fleets.size()); ++shp) {
	present.push_back(sys->planets[plan]->fleets[shp]);
	}
      }
    if(present.size() > 0) {
      for(int ctr=0; ctr<int(present.size()); ++ctr) {
	SDL_Rect rec2 = {0, 0, 4, 4};
	present[ctr]->SetPos(sys->xpos + 8,  sys->ypos + 6*ctr - 2);
	rec2.x = present[ctr]->XPos() - 2;
	rec2.y = present[ctr]->YPos() - 2;
	SDL_FillRect(screen, &rec2,
		color3(cur_game->players[present[ctr]->Owner()]->color));
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
      return;
      }
    }
  for(int flt=0; flt < int(cur_game->fleets.size()); ++flt) {
    offx = abs(cur_game->fleets[flt]->XPos() - mx);
    offy = abs(cur_game->fleets[flt]->YPos() - my);
    if(offx*offx + offy*offy <= 9) {
      cur_fleet = flt;
      panel = PANEL_FLEET;
      panel_init();
      return;
      }
    }
  }

void mouse_released_galaxy() {
  }

void mouse_moved_galaxy(int xp, int yp) {
  }
