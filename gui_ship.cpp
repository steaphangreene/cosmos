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
  }

void panel_redraw_ship(SDL_Rect *area) {
  }

void panel_clicked_ship(int mx, int my, int mb) {
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
