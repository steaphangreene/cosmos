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

void stats_draw_colony(Colony *, int update = 0);

static int selection = -1, grabbed = -1;
static int panel_offset = 0;

void gui_init_colony() {
  }

void panel_init_colony() {
  buttlist[PANEL_COLONY][BUTTON_EXIT] =	11;
  panel_offset = 0;
  selection = -1;
  grabbed = -1;
  buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =		0;
  buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	0;
  buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =		0;
  buttlist[PANEL_COLONY][BUTTON_BUILD] =		0;
  buttlist[PANEL_COLONY][BUTTON_ABANDON] =		0;
  }

void panel_cleanup_colony() {
  }

void panel_draw_colony() {
  Planet *plan = (Planet*)cur_object;
  if(plan->colonies.size() <= 0) return;
  Colony *col = plan->colonies[0];
  stats_draw_colony(col);
  }

void panel_clicked_colony(int mx, int my, int mb) {
  Planet *plan = (Planet*)cur_object;
  if(plan->colonies.size() <= 0) return;
  Colony *col = plan->colonies[0];

  if(mx >= 800 && mb == 4) {
    --panel_offset;
    stats_draw_colony(col, 0);
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = screenrec.h <? 24*(col->objs.size()+col->projs.size());
    update(&screenrec);
    return;
    }
  if(mx >= 800 && mb == 5) {
    ++panel_offset;
    stats_draw_colony(col, 0);
    SDL_Rect screenrec = {800, 12+24*SKIP, 224, 24*23};
    screenrec.h = screenrec.h <? 24*(col->objs.size()+col->projs.size());
    update(&screenrec);
    return;
    }

  int line = my;
  line -= 12+24*SKIP;
  if(line >= 0) line /= 24;
  if(line < 0 || line > 22
	|| line >= int(col->objs.size()) + int(col->projs.size())
		- panel_offset) {
    if(selection >= 0) {
      update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      selection = -1;
      stats_draw_colony(col);

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
    if(line < int(col->objs.size())) {
      do_dialog("%s\n\n%s",
	cur_tree->GetTech(col->objs[line])->names,
	cur_tree->GetTech(col->objs[line])->desc);
      }
    else {
      do_dialog("%s\n\n%s",
	cur_tree->GetTech(col->projs[line-int(col->objs.size())])->names,
	cur_tree->GetTech(col->projs[line-int(col->objs.size())])->desc);
      }
    }
  else if(mb == 1) {
    grabbed = line;
    if(selection != line) {
      if(selection >= 0)
	update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      selection = line;
      audio_play(click2, 8, 8);
      stats_draw_colony(col);
      update(800, 12+24*(SKIP+selection-panel_offset), 224, 24);
      if(line >= int(col->objs.size())) {
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
      else if(cur_tree->GetTech(col->objs[line])->type == TECH_PROJECT) {
	buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	9;
	buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	10;
	mo[BUTTON_CANCELPROJECT] = -1;
	mo[BUTTON_NEWPROJECT] = -1;
	buttlist[PANEL_COLONY][BUTTON_BUILD] =		0;
	buttlist[PANEL_COLONY][BUTTON_ABANDON] =		0;
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	}
      else if(cur_tree->GetTech(col->objs[line])->type == TECH_STRUCTURE) {
	buttlist[PANEL_COLONY][BUTTON_BUILD] =		9;
	buttlist[PANEL_COLONY][BUTTON_ABANDON] =		10;
	mo[BUTTON_BUILD] = -1;
	mo[BUTTON_ABANDON] = -1;
	buttlist[PANEL_COLONY][BUTTON_CANCELPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_NEWPROJECT] =	0;
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	}
      else if(cur_tree->GetTech(col->objs[line])->type == TECH_SHIP) {
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

void stats_draw_colony(Colony *col, int upd) {
  SDL_Rect screenrec = {800, 12, 224, 24*23};
  screenrec.h = screenrec.h <? 24*(col->objs.size()+col->projs.size()+SKIP);
  if(upd) {
    update(&screenrec);
    }
  SDL_FillRect(screen, &screenrec, black);

  int line = 0;
  char buf[80];
  int color = 7;
  if(col->Owner() >= 0) color = cur_game->players[col->Owner()]->color;

  line = 0;
  sprintf(buf, "Industry: %d/%d", col->SpareIndustry(), col->Industry());
  string_draw(screen, 816, 13+24*(line), cur_font[color], buf);

  if(panel_offset < 0) panel_offset = 0;
  if(panel_offset > (0 >? int(col->objs.size())+int(col->projs.size())-22))
    panel_offset = (0 >? int(col->objs.size())+int(col->projs.size())-22);
  line = -panel_offset;
  int indus = col->Industry(), need, used;
  for(int ctr=0; ctr<(int)col->objs.size() && line < 22; ++ctr,++line) {
    Tech *tc = cur_tree->GetTech(col->objs[ctr]);
    need = cur_tree->Upkeep(col->objs[ctr], col->oqty[ctr], col) >? 0;
    used = 0 >? (indus <? need);
    indus -= need;
    if(line >= 0) {
      int clr = color;
      if(selection == line+panel_offset) clr = 8;
      if(col->oqty[ctr] == 1) {
	sprintf(buf, "%d %s: %d/%d", col->oqty[ctr], tc->name, used, need);
	}
      else {
	sprintf(buf, "%d %s: %d/%d", col->oqty[ctr], tc->names, used, need);
	}
      string_draw(screen, 800, 13+24*(SKIP+line), cur_font[clr], buf);
      }
    }
  for(int ctr=0; ctr<(int)col->projs.size() && line < 22; ++ctr,++line) {
    Tech *tc;
    tc = cur_tree->GetTech(col->projs[ctr]);
    need = tc->icost >? 1;
    indus += col->prog[ctr];
    used = col->prog[ctr] >? (indus <? need);
    indus -= need;
    if(line >= 0) {
      int clr = color;
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

  Planet *plan = (Planet*)cur_object;
  if(plan->colonies.size() <= 0) return;
  Colony *col = plan->colonies[0];

  int line;
  line = (my - (24*SKIP+12))/24;
  line += panel_offset;
  if(grabbed < int(col->objs.size())) {
    line = panel_offset >? line;
    line = line <? int(col->objs.size()) - 1;
    if(mx >= 800 && line != grabbed) {
      int vec = (line-grabbed)/(abs(line-grabbed));  // Direction: 1 or -1
      for(int ctr=grabbed; ctr != line; ctr += vec) {
	int e1 = ctr <? (ctr + vec);
	int e2 = ctr >? (ctr + vec);
	int tmpo = col->objs[e1];
	int tmpq = col->oqty[e1];
	col->objs[e1] = col->objs[e2];
	col->oqty[e1] = col->oqty[e2];
	col->objs[e2] = tmpo;
	col->oqty[e2] = tmpq;
	}
      update(800, 12+24*(SKIP+(grabbed<?line)-panel_offset),
			224, 24*(1+abs(grabbed-line)));
      selection = line;
      grabbed = line;
      stats_draw_colony(col);
      audio_play(click2, 8, 8);
      }
    }
  else {
    line = int(col->objs.size()) >? panel_offset >? line;
    line = line <? int(col->projs.size()+col->objs.size())-1;
    if(mx >= 800 && line != grabbed) {
      line -= int(col->objs.size());
      grabbed -= int(col->objs.size());
      int vec = (line-grabbed)/(abs(line-grabbed));  // Direction: 1 or -1
      for(int ctr=grabbed; ctr != line; ctr += vec) {
	int e1 = ctr <? (ctr + vec);
	int e2 = ctr >? (ctr + vec);
	int tmp1 = col->projs[e1];
	int tmp2 = col->prog[e1];
	col->projs[e1] = col->projs[e2];
	col->prog[e1] = col->prog[e2];
	col->projs[e2] = tmp1;
	col->prog[e2] = tmp2;
	}
      line += int(col->objs.size());
      grabbed += int(col->objs.size());
      update(800, 12+24*(SKIP + (grabbed<?line) - panel_offset),
	224, 24*(1+abs(grabbed-line)));
      selection = line;
      grabbed = line;
      stats_draw_colony(col);
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
  Planet *plan = (Planet*)cur_object;
  if(plan->colonies.size() <= 0) return;
  Colony *col = plan->colonies[0];

  switch(button) {
    case(BUTTON_CANCELBUILD): {
      col->projs.erase(col->projs.begin()+(selection-int(col->objs.size())));
      col->prog.erase(col->prog.begin()+(selection-int(col->objs.size())));
      SDL_Rect endrec = {800,
	12+24*SKIP <? 12+(SKIP+selection-panel_offset)*24, 224, 0};
      endrec.h = 768-64*3-endrec.y;
      if(endrec.h > 0) {
	SDL_FillRect(screen, &endrec, black);
	update(&endrec);
	}
      if(selection >= int(col->objs.size())+int(col->projs.size())) {
	SDL_Rect canrec = {800, 768-64*2, 224, 64};
	SDL_FillRect(screen, &canrec, black);
	update(&canrec);
	buttlist[PANEL_COLONY][BUTTON_CANCELBUILD] =	0;
	selection = -1;
	}
      stats_draw_colony(col, 0);
      } break;
    case(BUTTON_ABANDON):
    case(BUTTON_CANCELPROJECT): {
      col->oqty[selection] = 0 >? (col->oqty[selection] - 1);
      stats_draw_colony(col, 1);
      } break;
    case(BUTTON_NEWPROJECT):
    case(BUTTON_BUILD): {
      col->projs.push_back(col->objs[selection]);
      col->prog.push_back(0);
      stats_draw_colony(col, 1);
      } break;
    }
  }

void panel_update_colony() {
  }
