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

#define SKIP 1

static int selection = -1, grabbed = -1;
static int panel_offset = 0;

void gui_init_fleet() {
  }

void panel_init_fleet() {
  buttlist[PANEL_FLEET][BUTTON_EXIT] =		11;
  panel_offset = 0;
  selection = -1;
  grabbed = -1;
  }

void panel_cleanup_fleet() {
  }

void panel_draw_fleet() {
  int line = 0;
  char buf[80];
  Fleet *flt = cur_fleet;
  int col = cur_game->players[flt->Owner()]->color;
  SDL_Rect screenrec = {800, 12, 224, 24*23};
  screenrec.h = screenrec.h <? 24*(flt->ships.size()+SKIP);
  SDL_FillRect(screen, &screenrec, black);

  sprintf(buf, "Fleet: %s", flt->name.c_str());
  string_draw(screen, 816, 13+24*(line++), cur_font[col], buf);

  for(int ctr=0; ctr < (int(flt->ships.size()) <? 22); ++ctr) {
    int clr = col;
    if(ctr == selection) clr = 8;
    sprintf(buf, "  %s", flt->ships[ctr]->name.c_str());
    string_draw(screen, 816, 13+24*(line++), cur_font[clr], buf);
    }

  if((!flt->Destination()) && flt->ships.size() > 1) {
    buttlist[PANEL_FLEET][BUTTON_SPLIT] =		9;
    }
  else {
    buttlist[PANEL_FLEET][BUTTON_SPLIT] =		0;
    }

  if(flt->CanLand() && page == PAGE_SYSTEM && (!flt->Destination())) {
    buttlist[PANEL_FLEET][BUTTON_LAND] =	10;
    mo[BUTTON_LAND] = -1;
    }
  else {
    buttlist[PANEL_FLEET][BUTTON_LAND] =	0;
    SDL_Rect canrec = {800, 768-64*2, 224, 64};
    SDL_FillRect(screen, &canrec, black);
    update(&canrec);
    }
  }

void panel_clicked_fleet(int mx, int my, int mb) {
  Fleet *flt = cur_fleet;
  if(mx >= 800 && mb == 4) {
    --panel_offset;
    panel_draw_fleet();
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = screenrec.h <? 24*(flt->ships.size());
    update(&screenrec);
    return;
    }
  if(mx >= 800 && mb == 5) {
    ++panel_offset;
    panel_draw_fleet();
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = screenrec.h <? 24*(flt->ships.size());
    update(&screenrec);
    return;
    }

  int line = my;
  line -= 12+24*SKIP;
  if(line >= 0) line /= 24;
  if(line < 0 || line > 22
        || line >= int(flt->ships.size()) - panel_offset) {
    if(selection >= 0) {
      update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      selection = -1;
      panel_draw_fleet();
      }
    return;
    }

  line += panel_offset;

  if(mb == 3) {
    }
  else if(mb == 1) {
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24};
    if(selection >= 0) {
      screenrec.y = 12+24*(SKIP+selection);
      update(&screenrec);
      }
    if(selection != line) {
      selection = line;
      screenrec.y = 12+24*(SKIP+selection);
      update(&screenrec);
      panel_draw_fleet();
      }
    else {
      cur_ship = line;
      panel = PANEL_SHIP;
      panel_init();
      }
    audio_play(click2, 8, 8);
    }
  }

void mouse_released_fleet() {
  grabbed = -1;
  }

void mouse_moved_fleet(int mx, int my) {
  }

void button_clicked_fleet(int button) {
  Fleet *flt = cur_fleet;
  if(button == BUTTON_LAND) {
    if(!flt->CanLand()) return;
    if(((Planet*)flt->Location())->colonies.size() < 1) {
      ((Planet*)flt->Location())->colonies.push_back(
	new Colony(flt->Owner(), ((Planet*)flt->Location()))
	);
      }

    int which = 0;
    while(!flt->ships[which]->CanLand()) ++which;
    ((Planet*)flt->Location())->colonies[0]->LandShip(flt->ships[which]);
    flt->ships.erase(flt->ships.begin()+which);
    if(flt->ships.size() < 1) {
      delete flt;
      panel = PANEL_GAME;
      cur_fleet = NULL;
      }
    panel_draw();
    page_draw();
    }
  if(button == BUTTON_SPLIT) {
    vector<Ship*>::iterator shp = cur_fleet->ships.begin()+1;
    for(; shp != cur_fleet->ships.end(); ++shp) {
      Fleet *newfleet = new Fleet(cur_fleet->Location(), cur_fleet->owner, cur_fleet->name.c_str());
      //newfleet->location = cur_fleet->location;
      newfleet->ships.push_back(*shp);
      newfleet->Location()->Sys()->fleets.push_back(newfleet);
      }
    cur_fleet->ships.erase(cur_fleet->ships.begin()+1, cur_fleet->ships.end());
    page_draw();
    panel_init();
    }
  }

void panel_update_fleet() {
  }
