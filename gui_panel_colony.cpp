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

void stats_draw_colony(Planet *, int update = 0);

static int selection = -1, grabbed = -1;
static int panel_offset = 0;

void gui_init_colony() {
  }

void panel_init_colony() {
  buttlist[PANEL_COLONY][BUTTON_EXIT] =	11;
  panel_offset = 0;
  selection = -1;
  grabbed = -1;
  }

void panel_cleanup_colony() {
  }

void panel_draw_colony() {
  Planet *plan;
  plan =cur_game->galaxys[cur_galaxy]->systems[cur_system]->planets[cur_planet];
  stats_draw_colony(plan);
  }

void panel_clicked_colony(int mx, int my, int mb) {
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];

  if(mx >= 800 && mb == 4) {
    --panel_offset;
    stats_draw_colony(plan, 0);
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = screenrec.h <? 24*(plan->objs.size()+plan->projs.size());
    update(&screenrec);
    return;
    }
  if(mx >= 800 && mb == 5) {
    ++panel_offset;
    stats_draw_colony(plan, 0);
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = screenrec.h <? 24*(plan->objs.size()+plan->projs.size());
    update(&screenrec);
    return;
    }

  int line = my;
  line -= 12+24*SKIP;
  if(line >= 0) line /= 24;
  if(line < 0 || line > 22
	|| line >= int(plan->objs.size()) + int(plan->projs.size())
		- panel_offset) {
    if(selection >= 0) {
      update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      selection = -1;
      stats_draw_colony(plan);

      SDL_Rect canrec = {800, 768-64*3, 224, 128};
      SDL_FillRect(screen, &canrec, black);
      buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
      buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	0;
      buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	0;
      buttlist[PANEL_COLONY][BUTTON_BUILD] =		0;
      buttlist[PANEL_COLONY][BUTTON_ABANDON] =		0;
      update(&canrec);
      }
    return;
    }

  line += panel_offset;

  if(mb == 3) {
    if(line < int(plan->objs.size())) {
      do_dialog("%s\n\n%s",
	cur_tree->GetTech(plan->objs[line])->names,
	cur_tree->GetTech(plan->objs[line])->desc);
      }
    else {
      do_dialog("%s\n\n%s",
	cur_tree->GetTech(plan->projs[line-int(plan->objs.size())])->names,
	cur_tree->GetTech(plan->projs[line-int(plan->objs.size())])->desc);
      }
    }
  else if(mb == 1) {
    grabbed = line;
    if(selection != line) {
      if(selection >= 0)
	update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      selection = line;
      audio_play(click2, 8, 8);
      stats_draw_colony(plan);
      update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      if(line >= int(plan->objs.size())) {
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	10;
	mo[BUTTON_CANCELBUILD] = -1;
	SDL_Rect canrec = {800, 768-64*3, 224, 64};
	SDL_FillRect(screen, &canrec, black);
	buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_BUILD] =		0;
	buttlist[PANEL_COLONY][BUTTON_ABANDON] =		0;
	update(&canrec);
	}
      else if(cur_tree->GetTech(plan->objs[line])->type == TECH_PROJECT) {
	buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	9;
	buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	10;
	mo[BUTTON_CANCELPROJECT] = -1;
	mo[BUTTON_NEWPROJECT] = -1;
	buttlist[PANEL_COLONY][BUTTON_BUILD] =		0;
	buttlist[PANEL_COLONY][BUTTON_ABANDON] =		0;
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	}
      else if(cur_tree->GetTech(plan->objs[line])->type == TECH_STRUCTURE) {
	buttlist[PANEL_COLONY][BUTTON_BUILD] =		9;
	buttlist[PANEL_COLONY][BUTTON_ABANDON] =		10;
	mo[BUTTON_BUILD] = -1;
	mo[BUTTON_ABANDON] = -1;
	buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	}
      else if(cur_tree->GetTech(plan->objs[line])->type == TECH_SHIP) {
	buttlist[PANEL_COLONY][BUTTON_BUILD] =		9;
	buttlist[PANEL_COLONY][BUTTON_ABANDON] =		10;
	mo[BUTTON_BUILD] = -1;
	mo[BUTTON_ABANDON] = -1;
	buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	}
      else {
	SDL_Rect canrec = {800, 768-64*3, 224, 128};
	SDL_FillRect(screen, &canrec, black);
	buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_BUILD] =		0;
	buttlist[PANEL_COLONY][BUTTON_ABANDON] =		0;
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	update(&canrec);
	}
      }
    }
  }

void stats_draw_colony(Planet *plan, int upd) {
  SDL_Rect screenrec = {800, 12, 224, 24*23};
  screenrec.h = screenrec.h <? 24*(plan->objs.size()+plan->projs.size()+SKIP);
  if(upd) {
    update(&screenrec);
    }
  SDL_FillRect(screen, &screenrec, black);

  int line = 0;
  char buf[80];
  int col = 7;
  if(plan->claimed >= 0) col = cur_game->players[plan->claimed]->color;


  line = 0;
  sprintf(buf, "Industry: %d/%d", plan->SpareIndustry(), plan->Industry());
  string_draw(screen, 816, 13+24*(line), cur_font[col], buf);

  if(panel_offset < 0) panel_offset = 0;
  if(panel_offset > (0 >? int(plan->objs.size())+int(plan->projs.size())-22))
    panel_offset = (0 >? int(plan->objs.size())+int(plan->projs.size())-22);
  line = -panel_offset;
  int indus = plan->Industry(), need, used;
  for(int ctr=0; ctr<(int)plan->objs.size() && line < 22; ++ctr,++line) {
    Tech *tc = cur_tree->GetTech(plan->objs[ctr]);
    need = cur_tree->Upkeep(plan->objs[ctr], plan->oqty[ctr], plan) >? 0;
    used = 0 >? (indus <? need);
    indus -= need;
    if(line >= 0) {
      int clr = col;
      if(selection == line+panel_offset) clr = 8;
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
    Tech *tc;
    tc = cur_tree->GetTech(plan->projs[ctr]);
    need = tc->icost >? 1;
    indus += plan->prog[ctr];
    used = 0 >? (indus <? need);
    indus -= need;
    if(line >= 0) {
      int clr = col;
      if(selection == line+panel_offset) clr = 8;
      sprintf(buf, "+%s: %d/%d", tc->name, used, need);
      string_draw(screen, 800, 13+24*(SKIP+line), cur_font[clr], buf);
      }
    }
  }

void mouse_released_colony() {
  grabbed = -1;
  }

void mouse_moved_colony(int mx, int my) {
  if(grabbed == -1) return;

  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  int line;
  line = (my - (24*SKIP+12))/24;
  line += panel_offset;
  if(grabbed < int(plan->objs.size())) {
    line = panel_offset >? line;
    line = line <? int(plan->objs.size()) - 1;
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
      update(800, 12+24*(SKIP+(grabbed<?line)-panel_offset),
			224, 24*(1+abs(grabbed-line)));
      selection = line;
      grabbed = line;
      stats_draw_colony(plan);
      audio_play(click2, 8, 8);
      }
    }
  else {
    line = int(plan->objs.size()) >? panel_offset >? line;
    line = line <? int(plan->projs.size()+plan->objs.size())-1;
    if(mx >= 800 && line != grabbed) {
      line -= int(plan->objs.size());
      grabbed -= int(plan->objs.size());
      int vec = (line-grabbed)/(abs(line-grabbed));  // Direction: 1 or -1
      for(int ctr=grabbed; ctr != line; ctr += vec) {
	int e1 = ctr <? (ctr + vec);
	int e2 = ctr >? (ctr + vec);
	int tmp1 = plan->projs[e1];
	int tmp2 = plan->prog[e1];
	plan->projs[e1] = plan->projs[e2];
	plan->prog[e1] = plan->prog[e2];
	plan->projs[e2] = tmp1;
	plan->prog[e2] = tmp2;
	}
      line += int(plan->objs.size());
      grabbed += int(plan->objs.size());
      update(800, 12+24*(SKIP + (grabbed<?line) - panel_offset),
	224, 24*(1+abs(grabbed-line)));
      selection = line;
      grabbed = line;
      stats_draw_colony(plan);
      audio_play(click2, 8, 8);
      }
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

void button_clicked_colony(int button) {
  System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
  Planet *plan = sys->planets[cur_planet];
  switch(button) {
    case(BUTTON_CANCELBUILD): {
      plan->projs.erase(plan->projs.begin()+(selection-int(plan->objs.size())));
      plan->prog.erase(plan->prog.begin()+(selection-int(plan->objs.size())));
      SDL_Rect endrec = {800,
	12+24*SKIP <? 12+(SKIP+selection-panel_offset)*24, 224, 0};
      endrec.h = 768-64*3-endrec.y;
      if(endrec.h > 0) {
	SDL_FillRect(screen, &endrec, black);
	update(&endrec);
	}
      if(selection >= int(plan->objs.size())+int(plan->projs.size())) {
	SDL_Rect canrec = {800, 768-64*2, 224, 64};
	SDL_FillRect(screen, &canrec, black);
	update(&canrec);
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	selection = -1;
	}
      stats_draw_colony(plan, 0);
      } break;
    case(BUTTON_ABANDON):
    case(BUTTON_CANCELPROJECT): {
      plan->oqty[selection] = 0 >? (plan->oqty[selection] - 1);
      stats_draw_colony(plan, 1);
      } break;
    case(BUTTON_NEWPROJECT):
    case(BUTTON_BUILD): {
      plan->projs.push_back(plan->objs[selection]);
      plan->prog.push_back(0);
      stats_draw_colony(plan, 1);
      } break;
    }
  }

void panel_update_colony() {
  }
