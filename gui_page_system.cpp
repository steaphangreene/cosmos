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
static int click_margin[SOBJECT_MAX] = {0};

extern int cheat1;

void gui_init_system() {
  star = get_star_image();
  splanet[0] = get_splanet_image();

  pagemap[PAGE_SYSTEM][BUTTON_EXIT] =		PAGE_GALAXY;

  click_margin[SOBJECT_PLANET] = 36;
  click_margin[SOBJECT_FLEET] = 9;
  }

void page_init_system() {
  }

void page_cleanup_system() {
  clear_sprites(1);
  }

void page_draw_system() {
  ++cur_game->frame;

  SDL_Rect destr = {(768-32)/2, (768-32)/2, 32, 32};
  SDL_BlitSurface(star, NULL, screen, &destr);
  System *sys = cur_system;

  if((!cheat1) && (!sys->KnownTo(local_player))) {
    string_drawc(screen, 384, 384-11, cur_font[4], "Unexplored");
    return;
    }

  int spnum = 10;
  clear_sprites(spnum);
  SDL_Rect fleetrec = {0, 0, 4, 4};
  SDL_Rect planrec = {0, 0, 3, 3};
  SDL_Rect claimrec = {0, 0, 9, 9};

  for(int obj=0; obj < int(sys->objects.size()); ++obj) {
    if((!cheat1) && (!sys->objects[obj]->SeenBy(local_player))) continue;

    if(sys->objects[obj]->Destination()
	&& sys->objects[obj]->Destination()->Sys() != sys)
      continue;

    if(sys->objects[obj]->SType() == SOBJECT_PLANET) {
      Planet *plan = (Planet*)sys->objects[obj];
      if((plan)->colonies.size() > 0) {
	claimrec.x = (plan)->SXPos(cur_game->turn) - 4;
	claimrec.y = (plan)->SYPos(cur_game->turn) - 4;
	SDL_FillRect(screen, &claimrec, color3(cur_game->players[
		(plan)->colonies[0]->Owner()]->color));
	}
      planrec.x = (plan)->SXPos(cur_game->turn) - 1;
      planrec.y = (plan)->SYPos(cur_game->turn) - 1;
      SDL_BlitSurface(splanet[0], NULL, screen, &planrec);
      }
    else if(sys->objects[obj]->SType() == SOBJECT_FLEET) {
      Fleet *flt = (Fleet*)sys->objects[obj];
      fleetrec.x = flt->SXPos(cur_game->turn) - 2;
      fleetrec.y = flt->SYPos(cur_game->turn) - 2;
      SDL_FillRect(screen, &fleetrec,
	color3(cur_game->players[flt->Owner()]->color));
      if(flt->Destination()) {
	SDL_Surface *line;
	update_sprite(spnum);
	line = getline(
	  flt->SXPos(cur_game->turn),
	  flt->SYPos(cur_game->turn),
	  flt->Destination()->SXPos(cur_game->turn),
	  flt->Destination()->SYPos(cur_game->turn),
	  0xFFFFFFFF, 0x0F0F0F0F
	  );
	set_sprite(spnum, line);
	move_sprite(spnum,
		flt->SXPos(cur_game->turn)
		<? flt->Destination()->SXPos(cur_game->turn),
		flt->SYPos(cur_game->turn)
		<? flt->Destination()->SYPos(cur_game->turn)
		);
	update_sprite(spnum++);
	}
      }
    }
  }

void page_update_system() {
  }

void page_clicked_system(int mx, int my, int mb) {
  if((!cheat1) && (!cur_system->KnownTo(local_player))) return;
  if(mb != 1 && mb != 3) return;

  if(mb == 1) {  // Deselect
    if(panel == PANEL_FLEET)  cur_object->SetCourse(NULL);
    clear_sprites(1, 10);
    panel = PANEL_GAME;
    }

  int offx, offy, sqd;
  System *sys = cur_system;
  for(int obj=0; obj < int(sys->objects.size()); ++obj) {
    if(sys->objects[obj]->OnFrame() != cur_game->frame) continue;
    offx = abs(sys->objects[obj]->SXPos(cur_game->turn) - mx);
    offy = abs(sys->objects[obj]->SYPos(cur_game->turn) - my);
    sqd = offx*offx + offy*offy;
    if(sqd <= click_margin[sys->objects[obj]->SType()]) {
      if(mb == 1) {
	clear_sprites(1, 10);
	audio_play(click2, 8, 8);
	cur_object = sys->objects[obj];
	if(cur_object->SType() == SOBJECT_PLANET) {
	  page = PAGE_PLANET;
	  panel = PANEL_COLONY;
	  }
	else if(cur_object->SType() == SOBJECT_FLEET) {
	  panel = PANEL_FLEET;
	  }
	panel_init();
	}
      else if(mb == 3 && panel == PANEL_FLEET) {
	if(sys->objects[obj] == cur_object->Location()
		&& (!cur_object->Destination()))
	  continue;
	if(cur_object->Target() && cur_object->Distance() >= 0) {
	  audio_play(click2, 8, 8);
	  cur_object->Engage();
	  clear_sprites(1, 10);
	  page_draw();
	  panel_draw();
	  }
	}
      return;
      }
    }
  if(mb == 1) {  // Deselect
    panel_init();
    }
  }

void mouse_released_system() {
  }

void mouse_moved_system(int mx, int my) {
  if((!cheat1) && (!cur_system->KnownTo(local_player))) return;
  if(panel != PANEL_FLEET) return;
  if(cur_game->frame != cur_object->OnFrame()) return;

  int offx, offy, sqd;
  System *sys = cur_system;
  for(int obj=0; obj < int(sys->objects.size()); ++obj) {
    if(sys->objects[obj]->OnFrame() != cur_game->frame
	|| sys->objects[obj] == cur_object
	|| sys->objects[obj] == cur_object->Destination()
	|| (sys->objects[obj] == cur_object->Location()
		&& (!cur_object->Destination())
		)
	)
      continue;
    offx = abs(sys->objects[obj]->SXPos(cur_game->turn) - mx);
    offy = abs(sys->objects[obj]->SYPos(cur_game->turn) - my);
    sqd = offx*offx + offy*offy;
    if(sqd <= click_margin[sys->objects[obj]->SType()]) {
      cur_object->SetCourse(sys->objects[obj]);
      panel_draw();

      Uint32 col = 0xFFFFFFFF;
      if(cur_object->Distance() < 0) col = 0xFF0000FF;
      else if(cur_object->Distance() >= 0) col = 0xFF00FF00;

      SDL_Surface *line;

      update_sprite(1);
      line = getline(
	cur_object->SXPos(cur_game->turn),
	cur_object->SYPos(cur_game->turn),
	((Planet*)sys->objects[obj])->SXPos(cur_game->turn),
	((Planet*)sys->objects[obj])->SYPos(cur_game->turn),
	col, 0x0F0F0F0F
	);
      set_sprite(1, line);
      move_sprite(1,
	cur_object->SXPos(cur_game->turn)
		<? ((Planet*)sys->objects[obj])->SXPos(cur_game->turn),
	cur_object->SYPos(cur_game->turn)
		<? ((Planet*)sys->objects[obj])->SYPos(cur_game->turn)
	);
      update_sprite(1);
      return;
      }
    }

  cur_object->SetCourse(NULL);
  clear_sprites(1, 10);
  panel_draw();
  }
