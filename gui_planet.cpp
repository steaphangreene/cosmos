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

extern unsigned long black;

extern SDL_Surface *screen;
extern font *cur_font[9];
extern int click1, click2;

static SDL_Surface *planet[4], *satellite[4];

extern int buttlist[PAGE_MAX][BUTTON_MAX];
extern int pagemap[PAGE_MAX][BUTTON_MAX];

extern int page;
extern int cur_galaxy;
extern int cur_system;
extern int cur_planet;

void cursor_draw();
void stats_draw_planet(Planet *);

int selection = -1;

void gui_init_planet() {
  planet[0] = get_alpha_image("graphics/planet00.raw", 768, 768);
  satellite[0] = get_alpha_image("graphics/moon00.raw", 64, 64);
  satellite[1] = get_alpha_image("graphics/moon01.raw", 64, 64);
  satellite[2] = get_alpha_image("graphics/satellite00.raw", 8, 8);

  buttlist[PAGE_PLANET][BUTTON_NEWPROJECT] =	10;
  buttlist[PAGE_PLANET][BUTTON_EXIT] =		11;
  pagemap[PAGE_PLANET][BUTTON_EXIT] =		PAGE_SYSTEM;
  }

void gui_button_clicked_planet(int button) {
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  switch(button) {
    case(BUTTON_CANCELPROJECT): {
      update(800, 12+24*(6+selection), 224, 24*(plan->objs.size()-selection));
      plan->objs[selection] = plan->objs[plan->objs.size()-1];
      plan->objs.pop_back();
      plan->oqty[selection] = plan->oqty[plan->oqty.size()-1];
      plan->oqty.pop_back();
      page_init_planet();
      SDL_Rect butrec = { 800, 576, 224, 64 };
      SDL_FillRect(screen, &butrec, black);
      stats_draw_planet(plan);
      } break;
    }
  }

void panel_redraw_planet(SDL_Rect *area) {
  Planet *plan;
  plan =cur_game->galaxys[cur_galaxy]->systems[cur_system]->planets[cur_planet];
  stats_draw_planet(plan);
  }

void page_redraw_planet(SDL_Rect *area) {
  SDL_Rect todo = *area;

  SDL_FillRect(screen, &todo, black);
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  SDL_BlitSurface(planet[plan->Type()], &todo, screen, &todo);

  SDL_Rect srcr = {0, 0, 64, 64};
  SDL_Rect destr = {0, 0, 64, 64};
  for(int sctr=0; sctr < plan->num_satellites; ++sctr) {
    Satellite *sat = plan->satellites[sctr];
    if(!(sat->InFront(cur_game->tick))) {
      srcr.x = sat->XPos(cur_game->tick) - 32;
      srcr.y = sat->YPos(cur_game->tick) - 32;
      destr.x = sat->XPos(cur_game->tick) - 32;
      destr.y = sat->YPos(cur_game->tick) - 32;
      if(overlaps(destr, todo)) {
	SDL_BlitSurface(satellite[sat->Type()], NULL, screen, &destr);
	SDL_BlitSurface(planet[plan->Type()], &srcr, screen, &destr);
	}
      }
    }
  for(int sctr=0; sctr < plan->num_satellites; ++sctr) {
    Satellite *sat = plan->satellites[sctr];
    if(sat->InFront(cur_game->tick)) {
      srcr.x = sat->XPos(cur_game->tick) - 32;
      srcr.y = sat->YPos(cur_game->tick) - 32;
      destr.x = sat->XPos(cur_game->tick) - 32;
      destr.y = sat->YPos(cur_game->tick) - 32;
      if(overlaps(destr, todo)) {
	SDL_BlitSurface(satellite[sat->Type()], NULL, screen, &destr);
	}
      }
    }
  todo = *area;
  SDL_SetClipRect(screen, NULL);
  }

static int lasttick = -1;

void page_init_planet() {
  buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	0;
  selection = -1;
  }

void page_draw_planet() {
  Planet *plan;
  SDL_Rect destr = {0, 0, 768, 768};
  plan =cur_game->galaxys[cur_galaxy]->systems[cur_system]->planets[cur_planet];
  SDL_BlitSurface(planet[plan->Type()], NULL, screen, &destr);
  lasttick = -1;
  page_update_planet();
  stats_draw_planet(plan);
  }

void page_update_planet() {
  SDL_Rect srcr = {0, 0, 64, 64};
  SDL_Rect destr = {0, 0, 64, 64};
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  if(lasttick != -1) {
    for(int sctr=0; sctr < plan->num_satellites; ++sctr) {
      Satellite *sat = plan->satellites[sctr];
      srcr.x = sat->XPos(lasttick) - 32;
      srcr.y = sat->YPos(lasttick) - 32;
      destr.x = sat->XPos(lasttick) - 32;
      destr.y = sat->YPos(lasttick) - 32;
      SDL_FillRect(screen, &destr, black);
      SDL_BlitSurface(planet[plan->Type()], &srcr, screen, &destr);
      update(&destr);
      }
    }
  for(int sctr=0; sctr < plan->num_satellites; ++sctr) {
    Satellite *sat = plan->satellites[sctr];
    if(!(sat->InFront(cur_game->tick))) {
      srcr.x = sat->XPos(cur_game->tick) - 32;
      srcr.y = sat->YPos(cur_game->tick) - 32;
      destr.x = sat->XPos(cur_game->tick) - 32;
      destr.y = sat->YPos(cur_game->tick) - 32;
      SDL_BlitSurface(satellite[sat->Type()], NULL, screen, &destr);
      SDL_BlitSurface(planet[plan->Type()], &srcr, screen, &destr);
      update(&destr);
      }
    }
  for(int sctr=0; sctr < plan->num_satellites; ++sctr) {
    Satellite *sat = plan->satellites[sctr];
    if(sat->InFront(cur_game->tick)) {
      srcr.x = sat->XPos(cur_game->tick) - 32;
      srcr.y = sat->YPos(cur_game->tick) - 32;
      destr.x = sat->XPos(cur_game->tick) - 32;
      destr.y = sat->YPos(cur_game->tick) - 32;
      SDL_BlitSurface(satellite[sat->Type()], NULL, screen, &destr);
      update(&destr);
      }
    }
  lasttick = cur_game->tick;
  }

void page_clicked_planet(int mx, int my, int mb) {
  }

void panel_clicked_planet(int mx, int my, int mb) {
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  int line = my;

  line -= (12+24*6);
  if(line < 0) return;
  line /= 24;
  if(line >= (int)plan->objs.size()) return;

  if(mb == 3) {
    do_dialog("%s\n\n%s",
	cur_tree->GetTech(plan->objs[line])->name,
	cur_tree->GetTech(plan->objs[line])->desc);
    }
  else if(mb == 1 && selection != line) {
    update(800, 12+24*(6+selection), 224, 24);
    selection = line;
    audio_play(click2, 8, 8);
    stats_draw_planet(plan);
    update(800, 12+24*(6+selection), 224, 24);
    buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	9;
    }
  }

void stats_draw_planet(Planet *plan) {
  SDL_Rect screenrec = {800, 12, 224, 768};
  screenrec.h = 24*(plan->objs.size()+7);
  SDL_FillRect(screen, &screenrec, black);

  char buf[80];
  int col = 7;
  if(plan->claimed >= 0) col = cur_game->players[plan->claimed]->color;

  int line = 0;
  sprintf(buf, "Avg. Temp: %d", plan->Temperature());
  string_draw(screen, 800, 12+24*(line++), cur_font[col], buf);

  sprintf(buf, "Surf. Rad: %d", plan->Radiation());
  string_draw(screen, 800, 12+24*(line++), cur_font[col], buf);

  sprintf(buf, "Minerals: %d", plan->Minerals());
  string_draw(screen, 800, 12+24*(line++), cur_font[col], buf);
  
  sprintf(buf, "Atmosphere: %d", plan->Atmosphere());
  string_draw(screen, 800, 12+24*(line++), cur_font[col], buf);

  sprintf(buf, "Satellites: %d", plan->num_satellites);
  string_draw(screen, 800, 12+24*(line++), cur_font[col], buf);

  if(plan->claimed >= 0) {
    ++line;

    int indus = 0;
    for(int ctr=0; ctr<(int)plan->objs.size(); ++ctr) {
      int clr = col;
      if(selection == line - 6) clr = 8;
      Tech *tc = cur_tree->GetTech(plan->objs[ctr]);
      indus += cur_tree->Industry(plan->objs[ctr], plan->oqty[ctr], indus, plan);
      sprintf(buf, "%3d %s: %d", indus >? 0, tc->name, plan->oqty[ctr]);
      string_draw(screen, 800, 12+24*(line++), cur_font[clr], buf);
      }
    }
  }
