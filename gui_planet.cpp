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

static SDL_Surface *planet[4], *satellite[4];

extern int buttlist[PAGE_MAX][BUTTON_MAX];
extern int pagemap[PAGE_MAX][BUTTON_MAX];

extern int page;
extern int cur_galaxy;
extern int cur_system;
extern int cur_planet;

void cursor_draw();
void stats_draw_planet(Planet *);
void stats_redraw_planet(Planet *, SDL_Rect *);

void gui_init_planet() {
  planet[0] = get_alpha_image("graphics/planet00.raw", 768, 768);
  satellite[0] = get_alpha_image("graphics/moon00.raw", 64, 64);
  satellite[1] = get_alpha_image("graphics/moon01.raw", 64, 64);
  satellite[2] = get_alpha_image("graphics/satellite00.raw", 8, 8);

  buttlist[PAGE_PLANET][BUTTON_EXIT] =		11;
  pagemap[PAGE_PLANET][BUTTON_EXIT] =		PAGE_SYSTEM;
  }

void gui_button_clicked_planet(int button) {
  }

void panel_redraw_planet(SDL_Rect *area) {
  Planet *plan;
  plan =cur_game->galaxys[cur_galaxy]->systems[cur_system]->planets[cur_planet];
  stats_redraw_planet(plan, area);
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
//      if(overlaps(destr, mouser)) cursor_draw();
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
//      if(overlaps(destr, mouser)) cursor_draw();
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
//      if(overlaps(destr, mouser)) cursor_draw();
      update(&destr);
      }
    }
  lasttick = cur_game->tick;
  }

void page_clicked_planet(int mx, int my, int mb) {
  }

static SDL_Surface *panel = NULL;

void stats_redraw_planet(Planet *plan, SDL_Rect *area) {
  SDL_Rect todo = *area;
  SDL_Rect parea = todo;
  parea.x -= 800;
  if(parea.x < 0) {
    todo.x -= parea.x;
    todo.w += parea.x;
    parea = todo;
    parea.x -= 800;
    }
  SDL_BlitSurface(panel, &parea, screen, &todo);
  }

void stats_draw_planet(Planet *plan) {
  SDL_Rect panelrec = {0, 0, 224, 768};
  SDL_Rect screenrec = {800, 0, 224, 768};

  if(panel == NULL) {
    SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,
	224, 768, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    panel = SDL_DisplayFormat(tmp);
    SDL_FreeSurface(tmp);
    }

  SDL_FillRect(panel, &panelrec, black);
  char buf[80];

  int line = 0;
  sprintf(buf, "Minerals: %d", plan->Minerals());
  string_draw(panel, 0, 12+24*(line++), cur_font[7], buf);
  
  sprintf(buf, "Atmosphere: %d", plan->Atmosphere());
  string_draw(panel, 0, 12+24*(line++), cur_font[7], buf);

  sprintf(buf, "Avg. Temp: %d", plan->Temperature());
  string_draw(panel, 0, 12+24*(line++), cur_font[7], buf);

  sprintf(buf, "Surf. Rad: %d", plan->Radiation());
  string_draw(panel, 0, 12+24*(line++), cur_font[7], buf);

  sprintf(buf, "Satellites: %d", plan->num_satellites);
  string_draw(panel, 0, 12+24*(line++), cur_font[7], buf);

  if(plan->claimed >= 0) {
    ++line;

    sprintf(buf, "Player %d", plan->claimed+1);
    string_draw(panel, 0, 12+24*(line++), cur_font[
		cur_game->players[plan->claimed]->color], buf);

    for(int ctr=0; ctr<(int)plan->objs.size(); ++ctr) {
      Tech *tc = cur_tree->GetTech(plan->objs[ctr]);
//      if(tc->type == TECH_PROJECT || tc->type == TECH_BUILDING) {
	sprintf(buf, "%s: %d", tc->name, plan->oqty[ctr]);
	string_draw(panel, 0, 12+24*(line++), cur_font[
		cur_game->players[plan->claimed]->color], buf);
//	}
      }
    }

  SDL_BlitSurface(panel, NULL, screen, &screenrec);
  }
