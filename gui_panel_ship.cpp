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

#define SKIP 3

static int selection = -1, grabbed = -1;
static int panel_offset = 0;

void gui_init_ship() {
  }

void panel_init_ship() {
  buttlist[PANEL_SHIP][BUTTON_EXIT] =	11;
  panel_offset = 0;
  selection = -1;
  grabbed = -1;
  }

void panel_cleanup_ship() {
  }

void panel_draw_ship() {
  int line = 0;
  char buf[80];
  Fleet *flt = cur_fleet;
  Ship *shp = flt->ships[cur_ship];
  int col = cur_game->players[shp->Owner()]->color;
  SDL_Rect screenrec = {800, 12, 224, 24*23};
  screenrec.h = screenrec.h <? 24*(shp->NumItems()+SKIP);
  SDL_FillRect(screen, &screenrec, black);

  sprintf(buf, "Fleet: %s", flt->name.c_str());
  string_draw(screen, 816, 13+24*(line++), cur_font[col], buf);

  sprintf(buf, "  Ship: %s", shp->name.c_str());
  string_draw(screen, 816, 13+24*(line++), cur_font[col], buf);

  sprintf(buf, "    Crew: %dM", shp->Crew());
  string_draw(screen, 816, 13+24*(line++), cur_font[col], buf);
  }

void panel_clicked_ship(int mx, int my, int mb) {
  Fleet *flt = cur_fleet;
//  Ship *shp = flt->ships[cur_ship];
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

  if(my >= 12 && my < 36) {
    panel = PANEL_FLEET;
    panel_init();
    }
  }

void mouse_released_ship() {
  grabbed = -1;
  }

void mouse_moved_ship(int mx, int my) {
  }

void button_clicked_ship(int button) {
  }

void panel_update_ship() {
  }
