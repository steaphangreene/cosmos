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
  int col = cur_game->players[cur_game->fleets[cur_fleet]->Owner()]->color;

  sprintf(buf, ":%s:", cur_game->fleets[cur_fleet]->name.c_str());
  string_draw(screen, 816, 13+24*(line++), cur_font[col], buf);

  for(int ctr=0; ctr < int(cur_game->fleets[cur_fleet]->ships.size()); ++ctr) {
    sprintf(buf, "%s", cur_game->fleets[cur_fleet]->ships[ctr]->name.c_str());
    string_draw(screen, 816, 13+24*(line++), cur_font[col], buf);
    }
  }

void panel_redraw_fleet(SDL_Rect *area) {
  }

void panel_clicked_fleet(int mx, int my, int mb) {
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
