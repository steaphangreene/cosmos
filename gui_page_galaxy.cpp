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
  }

void page_cleanup_galaxy() {
  clear_sprites(1);
  }

void page_draw_galaxy() {
  ++cur_game->frame;

  int spnum = 10;
  clear_sprites(spnum);

  SDL_Rect rec = {0, 0, 3, 3};
  for(int snum=0; snum < cur_game->galaxys[cur_galaxy]->num_systems; ++snum) {
    System *sys = cur_game->galaxys[cur_galaxy]->systems[snum];
    if((cheat1 || sys->KnownTo(local_player)) && sys->Owner() >= 0) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = sys->GXPos(cur_game->turn) - 4;
      rec2.y = sys->GYPos(cur_game->turn) - 4;
      SDL_FillRect(screen, &rec2, color3(
	cur_game->players[sys->Owner()]->color
	));
      }
    rec.x = sys->GXPos(cur_game->turn) - 1;
    rec.y = sys->GYPos(cur_game->turn) - 1;
    SDL_BlitSurface(gstar, NULL, screen, &rec);

    for(int obj = 0; obj < int(sys->objects.size()); ++obj) {
      if(sys->objects[obj]->GMove() && sys->objects[obj]->Owner() >= 0
		&& (cheat1 || sys->objects[obj]->SeenBy(local_player))) {
	SDL_Rect rec2 = {0, 0, 4, 4};
	rec2.x = sys->objects[obj]->GXPos(cur_game->turn) - 2;
	rec2.y = sys->objects[obj]->GYPos(cur_game->turn) - 2;
	SDL_FillRect(screen, &rec2, color3(
		cur_game->players[sys->objects[obj]->Owner()]->color
		));

	if(sys->objects[obj]->Destination()
		&& sys->objects[obj]->Destination()->Sys()
			!= sys->objects[obj]->Sys()) {
	  SDL_Surface *line;
	  update_sprite(spnum);
	  line = getline(
		sys->objects[obj]->GXPos(cur_game->turn),
		sys->objects[obj]->GYPos(cur_game->turn),
		sys->objects[obj]->Destination()->GXPos(cur_game->turn),
		sys->objects[obj]->Destination()->GYPos(cur_game->turn),
		0xFFFFFFFF, 0x0F0F0F0F
		);
	  set_sprite(spnum, line);
	  move_sprite(spnum,
		sys->objects[obj]->GXPos(cur_game->turn)
		<? sys->objects[obj]->Destination()->GXPos(cur_game->turn),
		sys->objects[obj]->GYPos(cur_game->turn)
		<? sys->objects[obj]->Destination()->GYPos(cur_game->turn)
		);
	  update_sprite(spnum++);
	  }
	}
      }
    }
  }

void page_update_galaxy() {
  }

void page_clicked_galaxy(int mx, int my, int mb) {
  if(mb != 1 && mb != 3) return;

  int offx, offy;
  for(int snum=0; snum < cur_game->galaxys[cur_galaxy]->num_systems; ++snum) {
    System *sys = cur_game->galaxys[cur_galaxy]->systems[snum];
    offx = abs(sys->GXPos(cur_game->turn) - mx);
    offy = abs(sys->GYPos(cur_game->turn) - my);
    if(offx*offx + offy*offy <= 36) {
      if(mb == 1) {
	audio_play(click2, 8, 8);
	cur_system = sys;
	page = PAGE_SYSTEM;
	return;
	}
      else if(panel == PANEL_FLEET && cur_object->Target()) {
	audio_play(click2, 8, 8);
	cur_object->Engage();
	clear_sprites(1, 10);
	page_draw();
	panel_draw();
	return;
	}
      }
    for(int obj = 0; obj < int(sys->objects.size()); ++obj) {
      if(sys->objects[obj]->OnFrame() != cur_game->frame) continue;
      offx = abs(sys->objects[obj]->GXPos(cur_game->turn) - mx);
      offy = abs(sys->objects[obj]->GYPos(cur_game->turn) - my);
      if(offx*offx + offy*offy <= 9
		&& (cheat1 || sys->objects[obj]->SeenBy(local_player))) {
	if(mb == 1) {
	  audio_play(click2, 8, 8);
	  cur_object = sys->objects[obj];
	  panel = PANEL_FLEET;
	  panel_init();
	  return;
	  }
	else {
	  }
	}
      }
    }
  }

void mouse_released_galaxy() {
  }

void mouse_moved_galaxy(int mx, int my) {
  int offx, offy;
  for(int snum=0; snum < cur_game->galaxys[cur_galaxy]->num_systems; ++snum) {
    System *sys = cur_game->galaxys[cur_galaxy]->systems[snum];
    offx = abs(sys->GXPos(cur_game->turn) - mx);
    offy = abs(sys->GYPos(cur_game->turn) - my);
    if(offx*offx + offy*offy <= 36) {
      if(panel == PANEL_FLEET && cur_object->OnFrame() == cur_game->frame) {
	if(cur_object->Destination()
		&& cur_object->Destination()->Sys() != cur_object->Sys())
	  continue;
	cur_object->SetCourse(sys);
	panel_draw();

	Uint32 col = 0xFFFFFFFF;
	if(cur_object->Distance() < 0) col = 0xFF0000FF;
	else if(cur_object->Distance() >= 0) col = 0xFF00FF00;

	update_sprite(1);
	SDL_Surface *line = getline(
		cur_object->GXPos(cur_game->turn),
		cur_object->GYPos(cur_game->turn),
		sys->GXPos(cur_game->turn),
		sys->GYPos(cur_game->turn),
		col, 0x0F0F0F0F
		);
	set_sprite(1, line);
	move_sprite(1,
		cur_object->GXPos(cur_game->turn) <? sys->GXPos(cur_game->turn),
		cur_object->GYPos(cur_game->turn) <? sys->GYPos(cur_game->turn)
		);
	update_sprite(1);
	}
      return;
      }
    }
  if(panel == PANEL_FLEET) {
    cur_object->SetCourse(NULL);
    clear_sprites(1, 10);
    panel_draw();
    }
  }
