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
  set_sprite(1, NULL);
  }

void page_draw_system() {
  SDL_Rect destr = {(768-32)/2, (768-32)/2, 32, 32};
  SDL_BlitSurface(star, NULL, screen, &destr);
  destr.w = 3;  destr.h = 3;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  ++cur_game->frame;
  SDL_Rect shiprec = {0, 0, 4, 4};
  for(int plan=0; plan < sys->num_planets; ++plan) {
    if(sys->planets[plan]->colonies.size() > 0) {
      SDL_Rect claimrec = {0, 0, 9, 9};
      claimrec.x = sys->planets[plan]->XPos(cur_game->turn) - 4;
      claimrec.y = sys->planets[plan]->YPos(cur_game->turn) - 4;
      SDL_FillRect(screen, &claimrec, color3(cur_game->players[
		sys->planets[plan]->colonies[0]->Owner()]->color));
      }
    destr.x = sys->planets[plan]->XPos(cur_game->turn) - 1;
    destr.y = sys->planets[plan]->YPos(cur_game->turn) - 1;
    SDL_BlitSurface(splanet[0], NULL, screen, &destr);
    for(int ctr=0; ctr < int(sys->planets[plan]->fleets.size()); ++ctr) {
      if(sys->planets[plan]->fleets[ctr]->ships.size() < 1) continue;
      sys->planets[plan]->fleets[ctr]->SetPos(
	sys->planets[plan]->XPos(cur_game->turn) + 8,
	sys->planets[plan]->YPos(cur_game->turn) + 6*ctr - 2,
	cur_game->frame
	);
      shiprec.x = sys->planets[plan]->fleets[ctr]->XPos() - 2;
      shiprec.y = sys->planets[plan]->fleets[ctr]->YPos() - 2;
      SDL_FillRect(screen, &shiprec, color3(cur_game->players[
		sys->planets[plan]->fleets[ctr]->Owner()]->color));
      }
    }
  for(int f=0; f < int(sys->fleets.size()); ++f) {
    Fleet *flt = sys->fleets[f];
    if(flt->ships.size() < 1) continue;
    Planet *p1 = flt->Location();
    Planet *p2 = flt->Destination();
    int tt = flt->TripTime(), dn = flt->Progress(), tg = tt-dn;
    flt->SetPos(
	( p1->XPos(cur_game->turn - dn) * tg
	  + p2->XPos(cur_game->turn + tg) * dn) / tt,
	( p1->YPos(cur_game->turn - dn) * tg
	  + p2->YPos(cur_game->turn + tg) * dn) / tt,
	cur_game->frame
	);
    shiprec.x = flt->XPos() - 2;
    shiprec.y = flt->YPos() - 2;
    SDL_FillRect(screen, &shiprec,
	color3(cur_game->players[flt->Owner()]->color));
    }
  }

void page_update_system() {
  }

void page_clicked_system(int mx, int my, int mb) {
  if(mb != 1 && mb != 3) return;

  int offx, offy, fr;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < sys->num_planets; ++plan) {
    offx = abs(sys->planets[plan]->XPos(cur_game->turn) - mx);
    offy = abs(sys->planets[plan]->YPos(cur_game->turn) - my);
    if(offx*offx + offy*offy <= 36) {
      if(mb == 1) {
	audio_play(click2, 8, 8);
	cur_planet = plan;
	page = PAGE_PLANET;
	panel = PANEL_COLONY;
	panel_init();
	}
      else if(mb == 3 && panel == PANEL_FLEET) {
	if(sys->planets[plan] == cur_game->fleets[cur_fleet]->Location())
	  continue;
	int sqd, tm;
	audio_play(click2, 8, 8);
	offx = abs(sys->planets[plan]->XPos(cur_game->turn)
		- cur_game->fleets[cur_fleet]->XPos());
	offy = abs(sys->planets[plan]->YPos(cur_game->turn)
		- cur_game->fleets[cur_fleet]->YPos());
	sqd = offx*offx + offy*offy;
	tm = cur_game->fleets[cur_fleet]->TimeToLocal(sqd);
	if(tm >= 0) {
	  cur_game->fleets[cur_fleet]->SetCourse(sys->planets[plan], tm);
	  set_sprite(1, NULL);
	  page_draw();
	  }
	}
      return;
      }
    }
  for(int flt=0; flt < int(cur_game->fleets.size()); ++flt) {
    if(flt == cur_fleet && panel == PANEL_FLEET) continue;
    offx = abs(cur_game->fleets[flt]->XPos() - mx);
    offy = abs(cur_game->fleets[flt]->YPos() - my);
    fr = cur_game->fleets[flt]->OnFrame();
    if(cur_game->frame == fr && offx*offx + offy*offy <= 9) {
      update_sprite(1);
      set_sprite(1, NULL);
      audio_play(click2, 8, 8);
      cur_fleet = flt;
      panel = PANEL_FLEET;
      panel_init();
      return;
      }
    }
  }

void mouse_released_system() {
  }

void mouse_moved_system(int mx, int my) {
  if(panel != PANEL_FLEET) return;
  if(cur_game->frame != cur_game->fleets[cur_fleet]->OnFrame()) return;

  int offx, offy, sqd, tm, fr;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < sys->num_planets; ++plan) {
    if(sys->planets[plan] == cur_game->fleets[cur_fleet]->Location()) continue;
    offx = abs(sys->planets[plan]->XPos(cur_game->turn) - mx);
    offy = abs(sys->planets[plan]->YPos(cur_game->turn) - my);
    sqd = offx*offx + offy*offy;
    if(sqd <= 36) {
      Uint32 col = 0xFFFFFFFF;
      offx = abs(sys->planets[plan]->XPos(cur_game->turn)
		- cur_game->fleets[cur_fleet]->XPos());
      offy = abs(sys->planets[plan]->YPos(cur_game->turn)
		- cur_game->fleets[cur_fleet]->YPos());
      sqd = offx*offx + offy*offy;

      tm = cur_game->fleets[cur_fleet]->TimeToLocal(sqd);
      if(tm < 0) col = 0xFF0000FF;
      else if(tm > 0) col = 0xFF00FF00;

      update_sprite(1);
      SDL_Surface *line = getline(
	cur_game->fleets[cur_fleet]->XPos(),
	cur_game->fleets[cur_fleet]->YPos(),
	sys->planets[plan]->XPos(cur_game->turn),
	sys->planets[plan]->YPos(cur_game->turn),
	col, 0x0F0F0F0F
	);
      set_sprite(1, line);
      move_sprite(1,
	cur_game->fleets[cur_fleet]->XPos()
		<? sys->planets[plan]->XPos(cur_game->turn),
	cur_game->fleets[cur_fleet]->YPos()
		<? sys->planets[plan]->YPos(cur_game->turn)
	);
      update_sprite(1);
      cur_planet = plan;
      return;
      }
    }
  for(int flt=0; flt < int(cur_game->fleets.size()); ++flt) {
    offx = abs(cur_game->fleets[flt]->XPos() - mx);
    offy = abs(cur_game->fleets[flt]->YPos() - my);
    sqd = offx*offx + offy*offy;
    fr = cur_game->fleets[flt]->OnFrame();
    if(cur_fleet != flt && cur_game->frame == fr && sqd <= 9) {
      Uint32 col = 0xFFFFFFFF;
      offx = abs(cur_game->fleets[flt]->XPos()
		- cur_game->fleets[cur_fleet]->XPos());
      offy = abs(cur_game->fleets[flt]->YPos()
		- cur_game->fleets[cur_fleet]->YPos());
      sqd = offx*offx + offy*offy;

      int tm = cur_game->fleets[cur_fleet]->TimeToLocal(sqd);
      if(tm < 0) col = 0xFF0000FF;
      else if(tm > 0) col = 0xFF00FF00;

      update_sprite(1);
      SDL_Surface *line = getline(
	cur_game->fleets[cur_fleet]->XPos(),
	cur_game->fleets[cur_fleet]->YPos(),
	cur_game->fleets[flt]->XPos(),
	cur_game->fleets[flt]->YPos(),
	col, 0x0F0F0F0F
	);
      set_sprite(1, line);
      move_sprite(1,
	cur_game->fleets[cur_fleet]->XPos()
		<? cur_game->fleets[flt]->XPos(),
	cur_game->fleets[cur_fleet]->YPos()
		<? cur_game->fleets[flt]->YPos()
	);
      update_sprite(1);
//      cur_fleet = flt;
      return;
      }
    }
  }
