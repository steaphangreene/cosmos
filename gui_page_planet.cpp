#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

extern int cheat1;

#include <SDL.h>

#include "gui.h"
#include "game.h"
#include "audio.h"
#include "fonts.h"
#include "graphics.h"
#include "gui_local.h"

static SDL_Surface *planet[4], *satellite[4];
void stats_draw_planet(Planet *, int update = 0);

void gui_init_planet() {
  planet[0] = get_alpha_image("graphics/planet00.raw", 768, 768);
  satellite[0] = get_alpha_image("graphics/moon00.raw", 64, 64);
  satellite[1] = get_alpha_image("graphics/moon01.raw", 64, 64);
  satellite[2] = get_alpha_image("graphics/moon02.raw", 64, 64);

  buttlist[PAGE_PLANET][BUTTON_EXIT] =		11;
  pagemap[PAGE_PLANET][BUTTON_EXIT] =		PAGE_SYSTEM;
  }

static int lasttick = -1;

void page_init_planet() {
  buttlist[PAGE_PLANET][BUTTON_NEWPROJECT] =	0;
  buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	0;
  buttlist[PAGE_PLANET][BUTTON_CANCELBUILD] =	0;
  buttlist[PAGE_PLANET][BUTTON_BUILD] =		0;
  buttlist[PAGE_PLANET][BUTTON_ABANDON] =	0;

  panel = PANEL_COLONY;
  }

void page_cleanup_planet() {
//  if(panel == PANEL_COLONY) panel = PANEL_NONE;
  }

void page_draw_planet() {
  Planet *plan;
  SDL_Rect destr = {0, 0, 768, 768};
  plan = (Planet*)cur_object;
  SDL_BlitSurface(planet[plan->Type()], NULL, screen, &destr);
  lasttick = -1;
  page_update_planet();
  stats_draw_planet(plan, 0);
  }

void page_update_planet() {
  SDL_Rect srcr = {0, 0, 64, 64};
  SDL_Rect destr = {0, 0, 64, 64};
  Planet *plan = (Planet*)cur_object;
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

void mouse_released_planet() {
  }

void mouse_moved_planet(int mx, int my) {
  }

void stats_draw_planet(Planet *plan, int upd) {
  SDL_Rect screenrectl = {0, 12, 192, 72};
  SDL_Rect screenrectr = {576, 12, 192, 72};
  SDL_Rect screenrecbl = {0, 768-24*3-12, 192, 72};
  SDL_Rect screenrecbr = {576, 768-24*3-12, 192, 72};
  if(upd) {
    update(&screenrectl);
    update(&screenrectr);
    update(&screenrecbl);
    update(&screenrecbr);
    }
  SDL_FillRect(screen, &screenrectl, black);
  SDL_FillRect(screen, &screenrectr, black);
  SDL_FillRect(screen, &screenrecbl, black);
  SDL_FillRect(screen, &screenrecbr, black);

  int line = 0;
  char buf[80];
  int col = 7;

  line = 0;
  if(cheat1 || plan->KnownTo(local_player))
    sprintf(buf, "Atmosphere: %d", plan->Atmosphere());
  else
    sprintf(buf, "Atmosphere: ?");
  string_draw(screen, 5, 13+24*(line++), cur_font_black[col], buf);

  if(cheat1 || plan->KnownTo(local_player))
    sprintf(buf, "Minerals: %d", plan->Minerals());
  else
    sprintf(buf, "Minerals: ?");
  string_draw(screen, 5, 13+24*(line++), cur_font_black[col], buf);
  
  if(cheat1 || plan->KnownTo(local_player))
    sprintf(buf, "Satellites: %d", plan->num_satellites);
  else
    sprintf(buf, "Satellites: ?");
  string_draw(screen, 5, 13+24*(line++), cur_font_black[col], buf);

  line = 0;
  sprintf(buf, "Planet %s", plan->Name());
  string_drawr(screen, 768, 13+24*(line++), cur_font_black[col], buf);

  if(cheat1 || plan->KnownTo(local_player))
    sprintf(buf, "Avg. Temp: %dC", plan->Temperature()-273);
  else
    sprintf(buf, "Avg. Temp: ?");
  string_drawr(screen, 768, 13+24*(line++), cur_font_black[col], buf);

  if(cheat1 || plan->KnownTo(local_player))
    sprintf(buf, "Surf. Rad: %d", plan->Radiation());
  else
    sprintf(buf, "Surf. Rad: ?");
  string_drawr(screen, 768, 13+24*(line++), cur_font_black[col], buf);


  if(plan->colonies.size() > 0) {
    line = 0;
    sprintf(buf, "Pop: %s", popstr(plan->colonies[0]->Population(), plan->colonies[0]->PopulationM()));
    string_draw(screen, 12, (768-24*3-11)+24*(line++), cur_font_black[col], buf);

    sprintf(buf, "Free: %s", popstr(plan->colonies[0]->FreePop(), plan->colonies[0]->FreePopM()));
    string_draw(screen, 12, (768-24*3-11)+24*(line++), cur_font_black[col], buf);

    sprintf(buf, "Growth: %s", popstr(plan->colonies[0]->Growth(), plan->colonies[0]->GrowthM()));
    string_draw(screen, 12, (768-24*3-11)+24*(line++), cur_font_black[col], buf);


    line = 0;
    sprintf(buf, "Loyalty: %d", plan->colonies[0]->Loyalty());
    string_drawr(screen, 768, (768-24*3-11)+24*(line++), cur_font_black[col], buf);

    sprintf(buf, "Security: %d", plan->colonies[0]->Security());
    string_drawr(screen, 768, (768-24*3-11)+24*(line++), cur_font_black[col], buf);

    sprintf(buf, "Happiness: %d", plan->colonies[0]->Happiness());
    string_drawr(screen, 768, (768-24*3-11)+24*(line++), cur_font_black[col], buf);
    }
  }
