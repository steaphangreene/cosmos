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
static int bpanel = -1;
static Fleet *bfleet = NULL;

extern int cheat1;

void gui_init_galaxy() {
  gstar = get_gstar_image();

  buttlist[PANEL_GAME][BUTTON_TURN] =		10;
  buttlist[PANEL_GAME][BUTTON_EXIT] =		11;
  pagemap[PAGE_GALAXY][BUTTON_EXIT] =		PAGE_ROOT;
//  ambient[PAGE_GALAXY] = audio_loadsound("sounds/ambient00.wav");
  }

void page_init_galaxy() {
  if(panel < PANEL_GAME) panel = PANEL_GAME;
  bfleet = cur_fleet;
  bpanel = panel;
  }

void page_cleanup_galaxy() {
  clear_sprites(1);
  }

void page_draw_galaxy() {
  SDL_Rect rec = {0, 0, 3, 3};
  for(int snum=0; snum < cur_game->galaxys[cur_galaxy]->num_systems; ++snum) {
    System *sys = cur_game->galaxys[cur_galaxy]->systems[snum];
    if((cheat1 || sys->ExploredBy(local_player)) && sys->Owner() > -1) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = sys->xpos - 4;
      rec2.y = sys->ypos - 4;
      SDL_FillRect(screen, &rec2, color3(
	cur_game->players[sys->Owner()]->color
	));
      }
    rec.x = sys->xpos - 1;
    rec.y = sys->ypos - 1;
    SDL_BlitSurface(gstar, NULL, screen, &rec);

    vector<SObject *> present;
    for(int fn = 0; fn < int(sys->fleets.size()); ++fn) {
      if(cheat1 || ((Fleet*)sys->fleets[fn])->DetectedBy(local_player))
        present.push_back(sys->fleets[fn]);
      }
    if(present.size() > 0) {
      for(int ctr=0; ctr<int(present.size()); ++ctr) {
	SDL_Rect rec2 = {0, 0, 4, 4};
	((Fleet*)present[ctr])->SetPos(sys->xpos + 8,  sys->ypos + 6*ctr - 2);
	rec2.x = present[ctr]->SXPos(cur_game->turn) - 2;
	rec2.y = present[ctr]->SYPos(cur_game->turn) - 2;
	SDL_FillRect(screen, &rec2,
		color3(cur_game->players[((Fleet*)present[ctr])->Owner()]->color));
	}
      }
    }
  }

void page_update_galaxy() {
  }

void page_clicked_galaxy(int mx, int my, int mb) {
  if(mb != 1) return;

  int offx, offy;
  for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
    offx = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - mx);
    offy = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - my);
    if(offx*offx + offy*offy <= 36) {
      audio_play(click2, 8, 8);
      cur_system = sys;
      page = PAGE_SYSTEM;
      return;
      }
    }
/*
  for(int flt=0; flt < int(cur_game->fleets.size()); ++flt) {
    if((!cheat1) && (!cur_game->fleets[flt]->DetectedBy(local_player))) continue;
    offx = abs(cur_game->fleets[flt]->SXPos(cur_game->turn) - mx);
    offy = abs(cur_game->fleets[flt]->SYPos(cur_game->turn) - my);
    if(offx*offx + offy*offy <= 9) {
      audio_play(click2, 8, 8);
      bfleet = cur_game->fleets[flt];
      bpanel = PANEL_FLEET;
      cur_fleet = cur_game->fleets[flt];
      panel = PANEL_FLEET;
      panel_init();
      return;
      }
    }
*/
  }

void mouse_released_galaxy() {
  }

void mouse_moved_galaxy(int mx, int my) {
  static int mouse_over = 0;
  int offx, offy;
/*
  for(int flt=0; flt < int(cur_game->fleets.size()); ++flt) {
    if((!cheat1) && (!cur_game->fleets[flt]->DetectedBy(local_player))) continue;
    offx = abs(cur_game->fleets[flt]->SXPos(cur_game->turn) - mx);
    offy = abs(cur_game->fleets[flt]->SYPos(cur_game->turn) - my);
    if(offx*offx + offy*offy <= 9) {
      if(cur_fleet != cur_game->fleets[flt] || panel != PANEL_FLEET) {
	if(!mouse_over) {
	  bfleet = cur_fleet;
	  bpanel = panel;
	  mouse_over = 1;
	  }
	if(bpanel == PANEL_FLEET && panel == PANEL_FLEET
		&& bfleet != cur_game->fleets[flt]) {
	  update_sprite(1);
	  SDL_Surface *line = getline(
		bfleet->SXPos(cur_game->turn),
		bfleet->SYPos(cur_game->turn),
		cur_game->fleets[flt]->SXPos(cur_game->turn),
		cur_game->fleets[flt]->SYPos(cur_game->turn),
		0xFFFFFFFF, 0x0F0F0F0F
		);
	  set_sprite(1, line);
	  move_sprite(1,
		bfleet->SXPos(cur_game->turn)
			<? cur_game->fleets[flt]->SXPos(cur_game->turn),
		bfleet->SYPos(cur_game->turn)
			<? cur_game->fleets[flt]->SYPos(cur_game->turn)
		);
	  update_sprite(1);
	  }
	cur_fleet = cur_game->fleets[flt];
	panel = PANEL_FLEET;
	panel_init();
	}
      return;
      }
    }
*/
  for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
    offx = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - mx);
    offy = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - my);
    if(offx*offx + offy*offy <= 36) {
      if(cur_system != sys || mouse_over) {
	if(mouse_over) {
	  panel = bpanel;
	  cur_fleet = bfleet;
	  panel_init();
	  mouse_over = 0;
	  }	
	if(panel == PANEL_FLEET) {
	  update_sprite(1);
	  SDL_Surface *line = getline(
		cur_fleet->SXPos(cur_game->turn),
		cur_fleet->SYPos(cur_game->turn),
		cur_game->galaxys[cur_galaxy]->systems[sys]->xpos,
		cur_game->galaxys[cur_galaxy]->systems[sys]->ypos,
		0xFFFFFFFF, 0x0F0F0F0F
		);
	  set_sprite(1, line);
	  move_sprite(1,
		cur_fleet->SXPos(cur_game->turn)
			<? cur_game->galaxys[cur_galaxy]->systems[sys]->xpos,
		cur_fleet->SYPos(cur_game->turn)
			<? cur_game->galaxys[cur_galaxy]->systems[sys]->ypos
		);
	  update_sprite(1);
	  }
	cur_system = sys;
	}
      return;
      }
    }
  if(mouse_over) {
    panel = bpanel;
    cur_fleet = bfleet;
    panel_init();
    mouse_over = 0;
    }
  }
