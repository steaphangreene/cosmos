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

extern int cheat1;

void gui_init_system() {
  star = get_star_image();
  splanet[0] = get_splanet_image();

  pagemap[PAGE_SYSTEM][BUTTON_EXIT] =		PAGE_GALAXY;
  }

void page_init_system() {
  }

void page_cleanup_system() {
  clear_sprites(1);
  }

void page_draw_system() {
  vector<Fleet *> present;
  vector<Fleet *>::iterator cur;
  SDL_Rect destr = {(768-32)/2, (768-32)/2, 32, 32};
  SDL_BlitSurface(star, NULL, screen, &destr);
  destr.w = 3;  destr.h = 3;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];

  if((!cheat1) && (!cur_game->galaxys[cur_galaxy]->systems[cur_system]->ExploredBy(local_player))) {
    string_drawc(screen, 384, 384-11, cur_font[4], "Unexplored");
    return;
    }

  for(int plan=0; plan < int(sys->objects.size()); ++plan) {
    if(((Planet*)sys->objects[plan])->colonies.size() > 0) {
      SDL_Rect claimrec = {0, 0, 9, 9};
      claimrec.x = ((Planet*)sys->objects[plan])->SXPos(cur_game->turn) - 4;
      claimrec.y = ((Planet*)sys->objects[plan])->SYPos(cur_game->turn) - 4;
      SDL_FillRect(screen, &claimrec, color3(cur_game->players[
		((Planet*)sys->objects[plan])->colonies[0]->Owner()]->color));
      }
    destr.x = ((Planet*)sys->objects[plan])->SXPos(cur_game->turn) - 1;
    destr.y = ((Planet*)sys->objects[plan])->SYPos(cur_game->turn) - 1;
    SDL_BlitSurface(splanet[0], NULL, screen, &destr);
    }
  for(int f=0; f < int(sys->fleets.size()); ++f) {
    if((!cheat1) && (!((Fleet*)sys->fleets[f])->DetectedBy(local_player))) continue;
    present.push_back(((Fleet*)sys->fleets[f]));
    }

  int spnum = 10;
  clear_sprites(spnum);
  SDL_Rect shiprec = {0, 0, 4, 4};

  ++cur_game->frame;

  for(cur = present.begin(); cur != present.end(); ++cur) {
    if((*cur)->Destination()) {
      Planet *p1 = (*cur)->Location();
      Planet *p2 = (*cur)->Destination();
      int tt = (*cur)->TripTime(), dn = (*cur)->Progress(), tg = tt-dn;
      (*cur)->SetPos(
	( p1->SXPos(cur_game->turn - dn) * tg
	  + p2->SXPos(cur_game->turn + tg) * dn) / tt,
	( p1->SYPos(cur_game->turn - dn) * tg
	  + p2->SYPos(cur_game->turn + tg) * dn) / tt,
	cur_game->frame
	);
      }
    else { // At a planet - no Destination()
      Planet *p1 = (*cur)->Location();
      (*cur)->SetPos(
	p1->SXPos(cur_game->turn) + 8,
	p1->SYPos(cur_game->turn) - 2,
	cur_game->frame
	);
      }

    int collide = 1;
    vector<Fleet *>::iterator oth;
    while(collide) {
      collide = 0;
      for(oth = present.begin(); oth != cur; ++oth) {
	if(abs((*cur)->SXPos(cur_game->turn) - (*oth)->SXPos(cur_game->turn)) < 6
		&& abs((*cur)->SYPos(cur_game->turn) - (*oth)->SYPos(cur_game->turn)) < 6) {
	  (*cur)->SetPos((*cur)->SXPos(cur_game->turn), (*cur)->SYPos(cur_game->turn)+6, cur_game->frame);
	  collide = 1;
	  break;
	  }
	}
      }

    shiprec.x = (*cur)->SXPos(cur_game->turn) - 2;
    shiprec.y = (*cur)->SYPos(cur_game->turn) - 2;
    SDL_FillRect(screen, &shiprec,
	color3(cur_game->players[(*cur)->Owner()]->color));
    }

  for(cur = present.begin(); cur != present.end(); ++cur) {
    if((*cur)->Destination()) {
      SDL_Surface *line;
      int tm = (*cur)->TripTime() - (*cur)->Progress();

      update_sprite(spnum);
      line = getline(
	(*cur)->SXPos(cur_game->turn),
	(*cur)->SYPos(cur_game->turn),
	(*cur)->Destination()->SXPos(cur_game->turn + tm),
	(*cur)->Destination()->SYPos(cur_game->turn + tm),
	0xFFFFFFFF, 0x0F0F0F0F
	);
      set_sprite(spnum, line);
      move_sprite(spnum,
	(*cur)->SXPos(cur_game->turn) <? (*cur)->Destination()->SXPos(cur_game->turn + tm),
	(*cur)->SYPos(cur_game->turn) <? (*cur)->Destination()->SYPos(cur_game->turn + tm)
	);
      update_sprite(spnum++);

      update_sprite(spnum);
      line = getline(
	(*cur)->Destination()->SXPos(cur_game->turn),
	(*cur)->Destination()->SYPos(cur_game->turn),
	(*cur)->Destination()->SXPos(cur_game->turn + tm),
	(*cur)->Destination()->SYPos(cur_game->turn + tm),
	0xFFFFFFFF, 0x0F0F0F0F
	);
      set_sprite(spnum, line);
      move_sprite(spnum,
	(*cur)->Destination()->SXPos(cur_game->turn)
		<? (*cur)->Destination()->SXPos(cur_game->turn + tm),
	(*cur)->Destination()->SYPos(cur_game->turn)
		<? (*cur)->Destination()->SYPos(cur_game->turn + tm)
	);
      update_sprite(spnum++);
      }
    }
  }

void page_update_system() {
  }

void page_clicked_system(int mx, int my, int mb) {
  if((!cheat1) && (!cur_game->galaxys[cur_galaxy]->systems[cur_system]->ExploredBy(local_player))) return;
  if(mb != 1 && mb != 3) return;

  int offx, offy, fr;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < int(sys->objects.size()); ++plan) {
    offx = abs(((Planet*)sys->objects[plan])->SXPos(cur_game->turn) - mx);
    offy = abs(((Planet*)sys->objects[plan])->SYPos(cur_game->turn) - my);
    if(offx*offx + offy*offy <= 36) {
      if(mb == 1) {
	audio_play(click2, 8, 8);
	cur_planet = plan;
	page = PAGE_PLANET;
	panel = PANEL_COLONY;
	panel_init();
	}
      else if(mb == 3 && panel == PANEL_FLEET) {
	if(((Planet*)sys->objects[plan]) == cur_fleet->Location()) continue;
	if(cur_fleet->Target() && cur_fleet->Distance() >= 0) {
	  audio_play(click2, 8, 8);
	  cur_fleet->Engage();
	  clear_sprites(1, 10);
	  panel = PANEL_GAME;
	  page_draw();
	  panel_init();
	  }
	}
      return;
      }
    }
  for(int flt=0; flt < int(sys->fleets.size()); ++flt) {
    if((!cheat1) && (!((Fleet*)sys->fleets[flt])->DetectedBy(local_player))) continue;
    if(sys->fleets[flt] == cur_fleet && panel == PANEL_FLEET) continue;
    offx = abs(sys->fleets[flt]->SXPos(cur_game->turn) - mx);
    offy = abs(sys->fleets[flt]->SYPos(cur_game->turn) - my);
    fr = sys->fleets[flt]->OnFrame();
    if(cur_game->frame == fr && offx*offx + offy*offy <= 9) {
      if(mb == 1) {
	clear_sprites(1, 10);
	audio_play(click2, 8, 8);
	cur_fleet = (Fleet*)sys->fleets[flt];
	panel = PANEL_FLEET;
	panel_init();
	return;
	}
      else if(mb == 3 && panel == PANEL_FLEET) {
	if(sys->fleets[flt] == cur_fleet) continue;
	if(((Fleet*)sys->fleets[flt])->Location() != cur_fleet->Location()) continue;
	audio_play(click2, 8, 8);
	vector<Ship*>::iterator shp = cur_fleet->ships.begin(); 
	for(; shp != cur_fleet->ships.end(); ++shp) {
	  ((Fleet*)sys->fleets[flt])->ships.push_back(*shp);
	  }
	Fleet *tmpf = cur_fleet;
	cur_fleet = (Fleet*)sys->fleets[flt];
	delete tmpf;
	page_draw();
	panel_init();
	return;
	}
      }
    }
  if(mb == 1) {  // Deselect
    clear_sprites(1, 10);
    panel = PANEL_GAME;
    panel_init();
    return;
    }
  }

void mouse_released_system() {
  }

void mouse_moved_system(int mx, int my) {
  if((!cheat1) && (!cur_game->galaxys[cur_galaxy]->systems[cur_system]->ExploredBy(local_player))) return;
  if(panel != PANEL_FLEET) return;
  if(cur_game->frame != cur_fleet->OnFrame()) return;

  int offx, offy, sqd, tm, fr;
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  for(int plan=0; plan < int(sys->objects.size()); ++plan) {
    if(((Planet*)sys->objects[plan]) == cur_fleet->Location()) continue;
    offx = abs(((Planet*)sys->objects[plan])->SXPos(cur_game->turn) - mx);
    offy = abs(((Planet*)sys->objects[plan])->SYPos(cur_game->turn) - my);
    sqd = offx*offx + offy*offy;
    if(sqd <= 36) {
      Uint32 col = 0xFFFFFFFF;
      tm = 1;
      for(int trip=0; ; ++trip) {
	offx = abs(((Planet*)sys->objects[plan])->SXPos(cur_game->turn + trip)
		- cur_fleet->SXPos(cur_game->turn));
	offy = abs(((Planet*)sys->objects[plan])->SYPos(cur_game->turn + trip)
		- cur_fleet->SYPos(cur_game->turn));
	sqd = offx*offx + offy*offy;

	tm = cur_fleet->TimeToLocal(sqd);
	if(abs(tm) <= trip) {
	  tm = (tm/abs(tm))*trip;
	  break;
	  }
	}
      cur_fleet->SetCourse(((Planet*)sys->objects[plan]), tm);
      if(cur_fleet->Distance() < 0) col = 0xFF0000FF;
      else if(cur_fleet->Distance() >= 0) col = 0xFF00FF00;

      SDL_Surface *line;

      update_sprite(1);
      line = getline(
	cur_fleet->SXPos(cur_game->turn),
	cur_fleet->SYPos(cur_game->turn),
	((Planet*)sys->objects[plan])->SXPos(cur_game->turn + tm),
	((Planet*)sys->objects[plan])->SYPos(cur_game->turn + tm),
	col, 0x0F0F0F0F
	);
      set_sprite(1, line);
      move_sprite(1,
	cur_fleet->SXPos(cur_game->turn)
		<? ((Planet*)sys->objects[plan])->SXPos(cur_game->turn + tm),
	cur_fleet->SYPos(cur_game->turn)
		<? ((Planet*)sys->objects[plan])->SYPos(cur_game->turn + tm)
	);
      update_sprite(1);

      update_sprite(2);
      line = getline(
	((Planet*)sys->objects[plan])->SXPos(cur_game->turn),
	((Planet*)sys->objects[plan])->SYPos(cur_game->turn),
	((Planet*)sys->objects[plan])->SXPos(cur_game->turn + tm),
	((Planet*)sys->objects[plan])->SYPos(cur_game->turn + tm),
	col, 0x0F0F0F0F
	);
      set_sprite(2, line);
      move_sprite(2,
	((Planet*)sys->objects[plan])->SXPos(cur_game->turn)
		<? ((Planet*)sys->objects[plan])->SXPos(cur_game->turn + tm),
	((Planet*)sys->objects[plan])->SYPos(cur_game->turn)
		<? ((Planet*)sys->objects[plan])->SYPos(cur_game->turn + tm)
	);
      update_sprite(2);

      cur_planet = plan;
      return;
      }
    }
  for(int flt=0; flt < int(sys->fleets.size()); ++flt) {
    if((!cheat1) && (!((Fleet*)sys->fleets[flt])->DetectedBy(local_player))) continue;
    offx = abs(sys->fleets[flt]->SXPos(cur_game->turn) - mx);
    offy = abs(sys->fleets[flt]->SYPos(cur_game->turn) - my);
    sqd = offx*offx + offy*offy;
    fr = sys->fleets[flt]->OnFrame();
    if(cur_fleet != sys->fleets[flt] && cur_game->frame == fr && sqd <= 9) {
      Uint32 col = 0xFFFFFFFF;
      offx = abs(sys->fleets[flt]->SXPos(cur_game->turn)
		- cur_fleet->SXPos(cur_game->turn));
      offy = abs(sys->fleets[flt]->SYPos(cur_game->turn)
		- cur_fleet->SYPos(cur_game->turn));
      sqd = offx*offx + offy*offy;

      int tm = cur_fleet->TimeToLocal(sqd);
      if(tm < 0) col = 0xFF0000FF;
      else if(tm > 0) col = 0xFF00FF00;

      update_sprite(1);
      SDL_Surface *line = getline(
	cur_fleet->SXPos(cur_game->turn),
	cur_fleet->SYPos(cur_game->turn),
	sys->fleets[flt]->SXPos(cur_game->turn),
	sys->fleets[flt]->SYPos(cur_game->turn),
	col, 0x0F0F0F0F
	);
      set_sprite(1, line);
      move_sprite(1,
	cur_fleet->SXPos(cur_game->turn)
		<? sys->fleets[flt]->SXPos(cur_game->turn),
	cur_fleet->SYPos(cur_game->turn)
		<? sys->fleets[flt]->SYPos(cur_game->turn)
	);
      update_sprite(1);

      update_sprite(2);
      set_sprite(2, NULL);
      update_sprite(2);

//      cur_fleet = sys->fleets[flt];
      return;
      }
    }
  }
