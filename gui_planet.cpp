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
extern int mo[BUTTON_MAX];

#define SKIP 1

extern int page;
extern int cur_galaxy;
extern int cur_system;
extern int cur_planet;

void cursor_draw();
void stats_draw_planet(Planet *, int update = 0);
char *popstr(int, int mpop=0);

int selection = -1, grabbed = -1;
static int panel_offset = 0;

void gui_init_planet() {
  panel_offset = 0;

  planet[0] = get_alpha_image("graphics/planet00.raw", 768, 768);
  satellite[0] = get_alpha_image("graphics/moon00.raw", 64, 64);
  satellite[1] = get_alpha_image("graphics/moon01.raw", 64, 64);
  satellite[2] = get_alpha_image("graphics/moon02.raw", 64, 64);

  buttlist[PAGE_PLANET][BUTTON_EXIT] =		11;
  pagemap[PAGE_PLANET][BUTTON_EXIT] =		PAGE_SYSTEM;
  }

void gui_button_clicked_planet(int button) {
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  switch(button) {
    case(BUTTON_ABANDON):
    case(BUTTON_CANCELPROJECT): {
      plan->oqty[selection] = 0 >? (plan->oqty[selection] - 1);
      stats_draw_planet(plan, 1);
      } break;
    case(BUTTON_NEWPROJECT):
    case(BUTTON_BUILD): {
      plan->projs.push_back(plan->objs[selection]);
      plan->prog.push_back(0);
      stats_draw_planet(plan, 1);
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
  buttlist[PAGE_PLANET][BUTTON_NEWPROJECT] =	0;
  buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	0;
  buttlist[PAGE_PLANET][BUTTON_BUILD] =		0;
  buttlist[PAGE_PLANET][BUTTON_ABANDON] =	0;
  selection = -1;
  grabbed = -1;
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
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  if(mx >= 800 && mb == 4) {
    --panel_offset;
    stats_draw_planet(plan, 1);
    }
  if(mx >= 800 && mb == 5) {
    ++panel_offset;
    stats_draw_planet(plan, 1);
    }
  }

void panel_clicked_planet(int mx, int my, int mb) {
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  int line = my;

  line -= 12+24*SKIP;
  if(line < 0) return;
  line /= 24;
  if(line >= (int)plan->objs.size()) return;

  if(mb == 3) {
    do_dialog("%s\n\n%s",
	cur_tree->GetTech(plan->objs[line])->names,
	cur_tree->GetTech(plan->objs[line])->desc);
    }
  else if(mb == 1) {
    grabbed = line;
    if(selection != line) {
      if(selection >= 0) update(800, 12+24*(SKIP+selection), 224, 24);
      selection = line;
      audio_play(click2, 8, 8);
      stats_draw_planet(plan);
      update(800, 12+24*(SKIP+selection), 224, 24);
      if(cur_tree->GetTech(plan->objs[line])->type == TECH_PROJECT) {
	buttlist[PAGE_PLANET][BUTTON_NEWPROJECT] =	9;
	buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	10;
	mo[BUTTON_CANCELPROJECT] = -1;
	mo[BUTTON_NEWPROJECT] = -1;
	buttlist[PAGE_PLANET][BUTTON_BUILD] =		0;
	buttlist[PAGE_PLANET][BUTTON_ABANDON] =		0;
	}
      else if(cur_tree->GetTech(plan->objs[line])->type == TECH_STRUCTURE) {
	buttlist[PAGE_PLANET][BUTTON_BUILD] =		9;
	buttlist[PAGE_PLANET][BUTTON_ABANDON] =		10;
	mo[BUTTON_BUILD] = -1;
	mo[BUTTON_ABANDON] = -1;
	buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	0;
	buttlist[PAGE_PLANET][BUTTON_NEWPROJECT] =	0;
	}
      else if(cur_tree->GetTech(plan->objs[line])->type == TECH_SHIP) {
	buttlist[PAGE_PLANET][BUTTON_BUILD] =		9;
	buttlist[PAGE_PLANET][BUTTON_ABANDON] =		10;
	mo[BUTTON_BUILD] = -1;
	mo[BUTTON_ABANDON] = -1;
	buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	0;
	buttlist[PAGE_PLANET][BUTTON_NEWPROJECT] =	0;
	}
      else {
        SDL_Rect canrec = {800, 768-64*3, 224, 128};
        SDL_FillRect(screen, &canrec, black);
	buttlist[PAGE_PLANET][BUTTON_CANCELPROJECT] =	0;
	buttlist[PAGE_PLANET][BUTTON_NEWPROJECT] =	0;
	buttlist[PAGE_PLANET][BUTTON_BUILD] =		0;
	buttlist[PAGE_PLANET][BUTTON_ABANDON] =		0;
        update(&canrec);
        }
      }
    }
  }

void stats_draw_planet(Planet *plan, int upd) {
  SDL_Rect screenrec = {800, 12, 224, 24*23};
  SDL_Rect screenrectl = {0, 12, 192, 72};
  SDL_Rect screenrectr = {576, 12, 192, 72};
  SDL_Rect screenrecbl = {0, 768-24*3-12, 192, 72};
  SDL_Rect screenrecbr = {576, 768-24*3-12, 192, 72};
  screenrec.h = screenrec.h <? 24*(plan->objs.size()+plan->projs.size()+SKIP);
  if(upd) {
    update(&screenrec);
    update(&screenrectl);
    update(&screenrectr);
    update(&screenrecbl);
    update(&screenrecbr);
    }
  SDL_FillRect(screen, &screenrec, black);
  SDL_FillRect(screen, &screenrectl, black);
  SDL_FillRect(screen, &screenrectr, black);
  SDL_FillRect(screen, &screenrecbl, black);
  SDL_FillRect(screen, &screenrecbr, black);

  int line = 0;
  char buf[80];
  int col = 7;
  if(plan->claimed >= 0) col = cur_game->players[plan->claimed]->color;


  line = 0;
  sprintf(buf, "Atmosphere: %d", plan->Atmosphere());
  string_draw(screen, 5, 13+24*(line++), cur_font[col], buf);

  sprintf(buf, "Minerals: %d", plan->Minerals());
  string_draw(screen, 5, 13+24*(line++), cur_font[col], buf);
  
  sprintf(buf, "Satellites: %d", plan->num_satellites);
  string_draw(screen, 5, 13+24*(line++), cur_font[col], buf);

  line = 0;
  sprintf(buf, "Avg. Temp: %d", plan->Temperature());
  string_drawr(screen, 768, 13+24*(line++), cur_font[col], buf);

  sprintf(buf, "Surf. Rad: %d", plan->Radiation());
  string_drawr(screen, 768, 13+24*(line++), cur_font[col], buf);


  if(plan->claimed >= 0) {
    line = 0;
    sprintf(buf, "Pop: %s", popstr(plan->Population(), plan->PopulationM()));
    string_draw(screen, 12, (768-24*3-11)+24*(line++), cur_font[col], buf);

    sprintf(buf, "Free: %s", popstr(plan->FreePop(), plan->FreePopM()));
    string_draw(screen, 12, (768-24*3-11)+24*(line++), cur_font[col], buf);

    sprintf(buf, "Growth: %s", popstr(plan->Growth(), plan->GrowthM()));
    string_draw(screen, 12, (768-24*3-11)+24*(line++), cur_font[col], buf);


    line = 0;
    sprintf(buf, "Loyalty: %d", plan->Loyalty());
    string_drawr(screen, 768, (768-24*3-11)+24*(line++), cur_font[col], buf);

    sprintf(buf, "Security: %d", plan->Security());
    string_drawr(screen, 768, (768-24*3-11)+24*(line++), cur_font[col], buf);

    sprintf(buf, "Happiness: %d", plan->Happiness());
    string_drawr(screen, 768, (768-24*3-11)+24*(line++), cur_font[col], buf);


    line = 0;
    sprintf(buf, "Industry: %d/%d", plan->SpareIndustry(), plan->Industry());
    string_draw(screen, 800, 13+24*(line), cur_font[col], buf);

    if(panel_offset < 0) panel_offset = 0;
    if(panel_offset > (0 >? int(plan->objs.size())+int(plan->projs.size())-22))
      panel_offset = (0 >? int(plan->objs.size())+int(plan->projs.size())-22);
    line = -panel_offset;
    int indus = plan->Industry(), need, used;
    for(int ctr=0; ctr<(int)plan->objs.size() && line < 22; ++ctr,++line) {
      if(line >= 0) {
	int clr = col;
	if(selection == line+panel_offset) clr = 8;
	Tech *tc = cur_tree->GetTech(plan->objs[ctr]);
	need = cur_tree->Upkeep(plan->objs[ctr], plan->oqty[ctr], plan) >? 0;
	used = 0 >? (indus <? need);
	indus -= need;
	if(plan->oqty[ctr] == 1) {
	  sprintf(buf, "%d %s: %d/%d", plan->oqty[ctr], tc->name, used, need);
	  }
	else {
	  sprintf(buf, "%d %s: %d/%d", plan->oqty[ctr], tc->names, used, need);
	  }
	string_draw(screen, 800, 13+24*(SKIP+line), cur_font[clr], buf);
	}
      }
    for(int ctr=0; ctr<(int)plan->projs.size() && line < 22; ++ctr,++line) {
      if(line >= 0) {
	Tech *tc;
	tc = cur_tree->GetTech(plan->projs[ctr]);
	need = tc->icost >? 1;
	indus += plan->prog[ctr];
	used = 0 >? (indus <? need);
	indus -= need;
	sprintf(buf, "+%s: %d/%d", tc->name, used, need);
	string_draw(screen, 800, 13+24*(SKIP+line), cur_font[col], buf);
	}
      }
    }
  }

void mouse_released_planet() {
  grabbed = -1;
  }

void mouse_moved_planet(int mx, int my) {
  if(grabbed == -1) return;

  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  int line;
  line = (my - (24*SKIP+12))/24;
  line = 0 >? line <? (plan->objs.size() - 1);
  if(mx >= 800 && line != grabbed) {
    int vec = (line-grabbed)/(abs(line-grabbed));  // Direction: 1 or -1
    for(int ctr=grabbed; ctr != line; ctr += vec) {
      int e1 = ctr <? (ctr + vec);
      int e2 = ctr >? (ctr + vec);
      int tmpo = plan->objs[e1];
      int tmpq = plan->oqty[e1];
      plan->objs[e1] = plan->objs[e2];
      plan->oqty[e1] = plan->oqty[e2];
      plan->objs[e2] = tmpo;
      plan->oqty[e2] = tmpq;
      }
    update(800, 12+24*(SKIP+(grabbed<?line)), 224, 24*(1+abs(grabbed-line)));
    selection = line;
    grabbed = line;
    stats_draw_planet(plan);
    audio_play(click2, 8, 8);
    }
  }


char *popstr(int pop, int mpop) {
  static char buf[64];

  pop += mpop / 1000000;
  mpop %= 1000000;

  if(abs(pop) == 0 && abs(mpop) < 1000) {
    sprintf(buf, "%d", mpop);
    }
  if(abs(pop) == 0 && abs(mpop) < 100000) {
    sprintf(buf, "%d.%dK", mpop/1000, abs((mpop%1000)/100));
    }
  else if(abs(pop) == 0) {
    sprintf(buf, "%dK", mpop/1000);
    }
  else if(abs(pop) < 10) {
    sprintf(buf, "%d.%.2dM", pop, abs(mpop/10000));
    }
  else if(abs(pop) < 1000) {
    sprintf(buf, "%dM", pop);
    }
  else {
    sprintf(buf, "%d.%dG", pop/1000, abs((pop%1000)/100));
    }
  return buf;
  }
