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
#include "position.h"

static SDL_Surface *gstar;

extern int cheat1;

void gui_init_galaxy() {
  gstar = get_gstar_image();

  buttlist[PANEL_GAME][BUTTON_TURN] =		10;
  buttlist[PANEL_GAME][BUTTON_EXIT] =		11;
  pagemap[PAGE_GALAXY][BUTTON_EXIT] =		PAGE_ROOT;
//  ambient[PAGE_GALAXY] = audio_loadsound("sounds/ambient00.wav");

  Fleet::SetSize(SOBJECT_FLEET, FLEET_SIZE);
  Fleet::SetSpace(SOBJECT_FLEET, FLEET_SPACE);
  Fleet::SetSqOff(SOBJECT_FLEET, FLEET_SQOFF);

  Planet::SetSize(SOBJECT_PLANET, PLANET_SIZE);
  Planet::SetSpace(SOBJECT_PLANET, PLANET_SPACE);
  Planet::SetSqOff(SOBJECT_PLANET, PLANET_SQOFF);

  System::SetSize(SOBJECT_SYSTEM, SYSTEM_SIZE);
  System::SetSpace(SOBJECT_SYSTEM, SYSTEM_SPACE);
  System::SetSqOff(SOBJECT_SYSTEM, SYSTEM_SQOFF);
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
  for(int snum=0; snum < int(cur_galaxy->systems.size()); ++snum) {
    System *sys = cur_galaxy->systems[snum];
    if((cheat1 || sys->KnownTo(local_player)) && sys->Owner() >= 0) {
      SDL_Rect rec2 = {0, 0, 9, 9};
      rec2.x = sys->GXPos() - 4;
      rec2.y = sys->GYPos() - 4;
      SDL_FillRect(screen, &rec2, color3(
	cur_game->players[sys->Owner()]->color
	));
      }
    rec.x = sys->GXPos() - 1;
    rec.y = sys->GYPos() - 1;
    SDL_BlitSurface(gstar, NULL, screen, &rec);

    for(int obj = 0; obj < int(sys->objects.size()); ++obj) {
      if(sys->objects[obj]->GMove() && sys->objects[obj]->Owner() >= 0
		&& (cheat1 || sys->objects[obj]->SeenBy(local_player))) {
	SDL_Rect rec2 = {0, 0, FLEET_SIZE, FLEET_SIZE};
	rec2.x = sys->objects[obj]->GXPos() - sys->objects[obj]->Size()/2;
	rec2.y = sys->objects[obj]->GYPos() - sys->objects[obj]->Size()/2;
	SDL_FillRect(screen, &rec2, color3(
		cur_game->players[sys->objects[obj]->Owner()]->color
		));

	if(cur_object == sys->objects[obj]) {
	  update_sprite(1);
	  set_sprite(1, selectg, 0);
	  int off = selectg->w/2;
	  move_sprite(1, cur_object->GXPos() - off, cur_object->GYPos() - off);
	  update_sprite(1);
	  }

	if(sys->objects[obj]->Destination()
		&& sys->objects[obj]->Destination()->Sys()
			!= sys->objects[obj]->Sys()) {
	  SDL_Surface *line;
	  update_sprite(spnum);
	  line = getline(
		sys->objects[obj]->GXPos(),
		sys->objects[obj]->GYPos(),
		sys->objects[obj]->Destination()->GXPos(),
		sys->objects[obj]->Destination()->GYPos(),
		0xFFFFFFFF, 0x0F0F0F0F
		);
	  set_sprite(spnum, line);
	  move_sprite(spnum,
		sys->objects[obj]->GXPos()
		<? sys->objects[obj]->Destination()->GXPos(),
		sys->objects[obj]->GYPos()
		<? sys->objects[obj]->Destination()->GYPos()
		);
	  update_sprite(spnum++);
	  }
	}
      }
    }

  int mx, my;
  SDL_GetMouseState(&mx, &my);
  mouse_moved_galaxy(mx, my);
  }

void page_update_galaxy() {
  }

void page_clicked_galaxy(int mx, int my, int mb) {
  if(mb != 1 && mb != 3) return;

  int offx, offy;
  for(int snum=0; snum < int(cur_galaxy->systems.size()); ++snum) {
    System *sys = cur_galaxy->systems[snum];
    offx = abs(sys->GXPos() - mx);
    offy = abs(sys->GYPos() - my);
    if(offx*offx + offy*offy <= sys->SqOff()) {
      if(mb == 1) {
	audio_play(click2, 8, 8);
	cur_system = sys;
	page = PAGE_SYSTEM;
	return;
	}
      else if(panel == PANEL_FLEET && cur_object->Target()) {
	audio_play(click2, 8, 8);
	cur_object->Engage();
	clear_sprites(2, 10);
	page_draw();
	panel_draw();
	return;
	}
      }
    for(int obj = 0; obj < int(sys->objects.size()); ++obj) {
      if(sys->objects[obj]->OnFrame() != cur_game->frame) continue;
      offx = abs(sys->objects[obj]->GXPos() - mx);
      offy = abs(sys->objects[obj]->GYPos() - my);
      if(offx*offx + offy*offy <= sys->objects[obj]->SqOff()
		&& (cheat1 || sys->objects[obj]->SeenBy(local_player))) {
	if(mb == 1) {
	  audio_play(click2, 8, 8);
	  cur_object = sys->objects[obj];
	  panel = PANEL_FLEET;
	  panel_init();

	  update_sprite(1);
	  set_sprite(1, selectg, 0);
	  int off = selectg->w/2;
	  move_sprite(1, cur_object->GXPos() - off, cur_object->GYPos() - off);
	  update_sprite(1);

	  return;
	  }
	else {
	  }
	}
      }
    }
  if(mb == 1) {  // Deselect
    if(panel == PANEL_FLEET)  cur_object->SetCourse(NULL);
    clear_sprites(1, 10);
    panel = PANEL_GAME;
    panel_init();
    }
  }

void mouse_released_galaxy() {
  }

void mouse_moved_galaxy(int mx, int my) {
  int offx, offy;
  for(int snum=0; snum < int(cur_galaxy->systems.size()); ++snum) {
    System *sys = cur_galaxy->systems[snum];
    offx = abs(sys->GXPos() - mx);
    offy = abs(sys->GYPos() - my);
    if(offx*offx + offy*offy <= sys->SqOff()) {
      if(panel == PANEL_FLEET && cur_object->OnFrame() == cur_game->frame) {
	if(cur_object->Destination()
		&& cur_object->Destination()->Sys() != cur_object->Sys())
	  continue;

	if((SObject*)(cur_object->Target()) == (SObject*)sys) return;

	cur_object->SetCourse(sys);
	panel_draw();

	Uint32 col = 0xFFFFFFFF;
	if(cur_object->Distance() < 0) col = 0xFF0000FF;
	else if(cur_object->Distance() >= 0) col = 0xFF00FF00;

	update_sprite(2);
	SDL_Surface *line = getline(
		cur_object->GXPos(),
		cur_object->GYPos(),
		sys->GXPos(),
		sys->GYPos(),
		col, 0x0F0F0F0F
		);
	set_sprite(2, line);
	move_sprite(2,
		cur_object->GXPos() <? sys->GXPos(),
		cur_object->GYPos() <? sys->GYPos()
		);
	update_sprite(2);
	}
      return;
      }
    }
  if(panel == PANEL_FLEET && cur_object->Target()) {
    cur_object->SetCourse(NULL);
    clear_sprites(2, 10);
    panel_draw();
    }
  }
