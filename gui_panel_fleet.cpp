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
  buttlist[PANEL_FLEET][BUTTON_EXIT] =	11;
  panel_offset = 0;
  selection = -1;
  grabbed = -1;
  }

void panel_cleanup_fleet() {
  }

void panel_draw_fleet() {
  int line = 0;
  char buf[80];
  Fleet *flt = cur_game->fleets[cur_fleet];
  int col = cur_game->players[flt->Owner()]->color;
  SDL_Rect screenrec = {800, 12, 224, 24*23};
  screenrec.h = screenrec.h <? 24*(flt->ships.size()+SKIP);
  SDL_FillRect(screen, &screenrec, black);

  sprintf(buf, "Fleet: %s", flt->name.c_str());
  string_draw(screen, 816, 13+24*(line++), cur_font[col], buf);

  for(int ctr=0; ctr < int(flt->ships.size()); ++ctr) {
    int clr = col;
    if(ctr == selection) clr = 8;
    sprintf(buf, "  %s", flt->ships[ctr]->name.c_str());
    string_draw(screen, 816, 13+24*(line++), cur_font[clr], buf);
    }
  }

void panel_clicked_fleet(int mx, int my, int mb) {
  Fleet *flt = cur_game->fleets[cur_fleet];
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
  }

void panel_update_fleet() {
  }
