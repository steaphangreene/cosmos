#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

#include <SDL.h>

#include "gui.h"
#include "game.h"
#include "audio.h"
#include "graphics.h"

int done = 0;

extern SDL_Surface *screen;
static SDL_Rect mouser = {-1, -1, 0, 0};
static SDL_Surface *intro, *cursor, *button[BUTTON_MAX][2];
static SDL_Surface *planet[4], *star;

int lastpage = PAGE_INVALID, page = PAGE_ROOT;
int buttlist[PAGE_MAX][BUTTON_MAX] = {{0}};
int pagemap[PAGE_MAX][BUTTON_MAX] = {{0}};

int mousex=0, mousey=0, mouseb=0;
int mo[BUTTON_MAX] = {0};

int ambient[PAGE_MAX] = {0}, click;
Sound *cur_amb=NULL;
int cur_amb_num=0;
int cur_galaxy=0;
int cur_system=0;
int cur_planet=0;

void gui_button_clicked(int button);
void cursor_draw();
void page_clicked(int mx, int my, int mb);
void page_draw();
void page_update();
void page_redraw(SDL_Rect *);

int update_buttons() {
  int inbutt = 0;
  mouseb = SDL_GetMouseState(&mousex, &mousey);
  for(int ctr=1; ctr < BUTTON_MAX; ++ctr) if(buttlist[page][ctr]) {
    int ord = buttlist[page][ctr];
    int m = (mousey >= 64*ord && mousex >= 812
	&& mousey < 50+64*ord && mousex < 1012 && !(mouseb));
    if(m != mo[ctr]) {
      mo[ctr] = m;
      SDL_Rect rect = { 812, 64*ord, 200, 50 };
      SDL_BlitSurface(button[ctr][mo[ctr]], NULL, screen, &rect);
      SDL_UpdateRect(screen, 812, 64*ord, 200, 50);
      }
    if(mousey >= 64*ord && mousex >= 812 && mousey < 50+64*ord && mousex < 1012)
      inbutt = ctr;
    }
  return inbutt;
  }

void page_init() {
  page_draw();

  buttlist[PAGE_ROOT][BUTTON_RESUMEGAME] = (cur_game->InProgress())? 6 : 0;

  if(ambient[page] && ambient[page] != cur_amb_num &&
	ambient[page] != ambient[lastpage]) {
    audio_stop(cur_amb);
    cur_amb = audio_loop(ambient[page], 8, 0);
    cur_amb_num = ambient[page];
    }

  lastpage = page;
  }

void gui_main() {
  int curbutt=0;
  SDL_Event event;
  while(!done) {
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT) {
      done = 1;
      }
    else if(event.type == SDL_VIDEOEXPOSE) {
      page_draw();
      }
    else if(event.type == SDL_USEREVENT) {
      ++cur_game->tick;
      if(cur_game->tick > 65535) cur_game->tick = 0;
      page_update();
      }
    else if(event.type == SDL_KEYDOWN) {
      if(event.key.keysym.sym == SDLK_ESCAPE) {
        if(pagemap[page][BUTTON_CANCEL]) {
	  page = pagemap[page][BUTTON_CANCEL];
	  }
        else if(pagemap[page][BUTTON_EXIT]) {
	  page = pagemap[page][BUTTON_EXIT];
	  }
        else if(buttlist[page][BUTTON_QUITGAME]) {
	  done = 1;
	  }
	}
      else if(event.key.keysym.sym == SDLK_RETURN) {
	if(event.key.keysym.mod & (KMOD_LALT|KMOD_RALT)) {
	  SDL_WM_ToggleFullScreen(screen);
	  SDL_WarpMouse(mousex, mousey);
	  }
	}
      }
    else if(event.type == SDL_MOUSEMOTION) {
      if(curbutt != update_buttons()) curbutt = 0;
      if(mouser.x != -1) page_redraw(&mouser);
      cursor_draw();
      }
    else if(event.type == SDL_MOUSEBUTTONDOWN) {
      curbutt = update_buttons();
      if(curbutt) {
	audio_play(click, 8, 8);
	}
      else if(mousex < 768) {
	page_clicked(mousex, mousey, event.button.button);
	}
      }
    else if(event.type == SDL_MOUSEBUTTONUP) {
      if(curbutt == update_buttons() && curbutt) {
	gui_button_clicked(curbutt);
        if(pagemap[page][curbutt]) {
	  page = pagemap[page][curbutt];
	  }
	}
      }

    if(page != lastpage) {
      page_init();
      }
    }
  }

void gui_init() {
  cursor = get_cursor_image();
  intro = get_image("graphics/intro.raw", 800, 768);

  planet[0] = get_alpha_image("graphics/planet00.raw", 768, 768);
  planet[1] = get_alpha_image("graphics/planet01.raw", 768, 768);
  planet[2] = get_alpha_image("graphics/planet02.raw", 768, 768);
  planet[3] = get_alpha_image("graphics/planet03.raw", 768, 768);
  star = get_star_image();

  button[BUTTON_RESUMEGAME][0] = build_button0("Resume Game");;
  button[BUTTON_RESUMEGAME][1] = build_button1("Resume Game");
  button[BUTTON_LOADGAME][0] = build_button0("Load Game");;
  button[BUTTON_LOADGAME][1] = build_button1("Load Game");
  button[BUTTON_NEWGAME][0] = build_button0("Create New Game");;
  button[BUTTON_NEWGAME][1] = build_button1("Create New Game");
  button[BUTTON_NETWORKGAME][0] = build_button0("Network Game");;
  button[BUTTON_NETWORKGAME][1] = build_button1("Network Game");
  button[BUTTON_SYSTEMOPTIONS][0] = build_button0("System Options");;
  button[BUTTON_SYSTEMOPTIONS][1] = build_button1("System Options");
  button[BUTTON_QUITGAME][0] = build_button0("Quit Game");;
  button[BUTTON_QUITGAME][1] = build_button1("Quit Game");
  button[BUTTON_CANCEL][0] = build_button0("Cancel");;
  button[BUTTON_CANCEL][1] = build_button1("Cancel");
  button[BUTTON_ACCEPT][0] = build_button0("Accept");;
  button[BUTTON_ACCEPT][1] = build_button1("Accept");
  button[BUTTON_OPTIONS][0] = build_button0("Options");;
  button[BUTTON_OPTIONS][1] = build_button1("Options");
  button[BUTTON_EXIT][0] = build_button0("Exit");;
  button[BUTTON_EXIT][1] = build_button1("Exit");
  button[BUTTON_RESETALL][0] = build_button0("Reset All");;
  button[BUTTON_RESETALL][1] = build_button1("Reset All");
  button[BUTTON_RANDOMIZE][0] = build_button0("Randomize");
  button[BUTTON_RANDOMIZE][1] = build_button1("Randomize");

  buttlist[PAGE_ROOT][BUTTON_LOADGAME] = 7;
  buttlist[PAGE_ROOT][BUTTON_NEWGAME] =		8;
  buttlist[PAGE_ROOT][BUTTON_NETWORKGAME] =	9;
  buttlist[PAGE_ROOT][BUTTON_SYSTEMOPTIONS] =	10;
  buttlist[PAGE_ROOT][BUTTON_QUITGAME] =	11;
  pagemap[PAGE_ROOT][BUTTON_RESUMEGAME] =	PAGE_GALAXY;
  pagemap[PAGE_ROOT][BUTTON_NEWGAME] =		PAGE_NEW;
  pagemap[PAGE_ROOT][BUTTON_LOADGAME] =		PAGE_LOAD;
  pagemap[PAGE_ROOT][BUTTON_NETWORKGAME] =	PAGE_NET;
  pagemap[PAGE_ROOT][BUTTON_SYSTEMOPTIONS] =	PAGE_SYSOPT;
  ambient[PAGE_ROOT] = audio_loadsound("sounds/ambient00.wav");

  buttlist[PAGE_NEW][BUTTON_RESETALL] =		8;
  buttlist[PAGE_NEW][BUTTON_RANDOMIZE] =	9;
  buttlist[PAGE_NEW][BUTTON_ACCEPT] =		10;
  buttlist[PAGE_NEW][BUTTON_CANCEL] =		11;
  pagemap[PAGE_NEW][BUTTON_ACCEPT] =		PAGE_GALAXY;
  pagemap[PAGE_NEW][BUTTON_CANCEL] =		PAGE_ROOT;

  buttlist[PAGE_LOAD][BUTTON_ACCEPT] =		10;
  buttlist[PAGE_LOAD][BUTTON_CANCEL] =		11;
//  pagemap[PAGE_LOAD][BUTTON_ACCEPT] =		PAGE_GALAXY;
  pagemap[PAGE_LOAD][BUTTON_CANCEL] =		PAGE_ROOT;

  buttlist[PAGE_NET][BUTTON_CANCEL] =		11;
  pagemap[PAGE_NET][BUTTON_CANCEL] =		PAGE_ROOT;

  buttlist[PAGE_SYSOPT][BUTTON_ACCEPT] =	10;
  buttlist[PAGE_SYSOPT][BUTTON_CANCEL] =	11;
  pagemap[PAGE_SYSOPT][BUTTON_ACCEPT] =		PAGE_ROOT;
  pagemap[PAGE_SYSOPT][BUTTON_CANCEL] =		PAGE_ROOT;
//  ambient[PAGE_SYSOPT] = audio_loadsound("sounds/ambient00.wav");

  buttlist[PAGE_GALAXY][BUTTON_EXIT] =		11;
  pagemap[PAGE_GALAXY][BUTTON_EXIT] =		PAGE_ROOT;
//  ambient[PAGE_GALAXY] = audio_loadsound("sounds/ambient00.wav");

  buttlist[PAGE_SYSTEM][BUTTON_EXIT] =		11;
  pagemap[PAGE_SYSTEM][BUTTON_EXIT] =		PAGE_GALAXY;

  buttlist[PAGE_PLANET][BUTTON_EXIT] =		11;
  pagemap[PAGE_PLANET][BUTTON_EXIT] =		PAGE_SYSTEM;

  click = audio_loadsound("sounds/click.wav");

  page_init();
  }

void gui_button_clicked(int button) {
  switch(page) {
    case(PAGE_ROOT): {
      switch(button) {
	case(BUTTON_QUITGAME): {
	  done = 1;
	  } break;
	}
      } break;
    case(PAGE_NEW): {
      switch(button) {
	case(BUTTON_RANDOMIZE): {
	  cur_game->Randomize();
	  page_init();
	  } break;
	case(BUTTON_RESETALL): {
	  cur_game->Reset();
	  page_init();
	  } break;
	}
      } break;
    }
  }

void cursor_draw() {
  mouser.x = mousex;
  mouser.y = mousey;
  mouser.w = cursor->w <? (1024-mousex);
  mouser.h = cursor->h <? (768-mousey);
  SDL_Rect cursorr = {0, 0, mouser.w, mouser.h};

  SDL_BlitSurface(cursor, &cursorr, screen, &mouser);
  SDL_UpdateRects(screen, 1, &mouser);
  }

void page_redraw(SDL_Rect *area) {
//  MUST NOT CALL "page_update" - MOUSE CURSOR LOOP!!!!
//  printf("Drawing (%d,%d) %dx%d\n", area->x, area->y, area->w, area->h);
  SDL_Rect todo = *area;
  if(area->x < 800) {
    if(page == PAGE_ROOT) {
      SDL_BlitSurface(intro, &todo, screen, &todo);
      todo = *area;
      }
    else if(page == PAGE_GALAXY) {
      SDL_FillRect(screen, &todo, 0x00000000);
      todo = *area;
      SDL_Rect rec = {0, 0, 3, 3};
      for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
	rec.x = cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - 1;
	rec.y = cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - 1;
	if(overlaps(rec, todo)) SDL_FillRect(screen, &rec, 0xFFFFFFFF);
	}
      }
    else if(page == PAGE_SYSTEM) {
      SDL_FillRect(screen, &todo, 0x00000000);
      SDL_Rect destr = {(768-32)/2, (768-32)/2, 32, 32};
      if(overlaps(destr, todo)) SDL_BlitSurface(star, NULL, screen, &destr);
      destr.w = 3;  destr.h = 3;
      System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
      for(int plan=0; plan < sys->num_planets; ++plan) {
	destr.x = sys->planets[plan]->XPos(cur_game->turn) - 1;
	destr.y = sys->planets[plan]->YPos(cur_game->turn) - 1;
	if(overlaps(destr, todo)) SDL_FillRect(screen, &destr, 0xFFFF00FF);
	}
      todo = *area;
      }
    else if(page == PAGE_PLANET) {
      SDL_FillRect(screen, &todo, 0x00000000);
      System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
      int ptype = sys->planets[cur_planet]->type;
      SDL_BlitSurface(planet[ptype], &todo, screen, &todo);

      SDL_Rect srcr = {0, 0, 3, 3};
      SDL_Rect destr = {0, 0, 3, 3};
      Planet *plan = sys->planets[cur_planet];
      for(int sat=0; sat < plan->num_satellites; ++sat) {
	srcr.x = plan->satellites[sat]->XPos(cur_game->tick) - 1;
	srcr.y = plan->satellites[sat]->YPos(cur_game->tick) - 1;
	destr.x = plan->satellites[sat]->XPos(cur_game->tick) - 1;
	destr.y = plan->satellites[sat]->YPos(cur_game->tick) - 1;
	if(overlaps(destr, todo)) {
	  SDL_FillRect(screen, &destr, 0xFFFFFF00);
	  if(plan->satellites[sat]->InFront(cur_game->tick))
	    SDL_BlitSurface(planet[ptype], &srcr, screen, &destr);
	  }
	}
      todo = *area;
      }
    else {
      SDL_FillRect(screen, &todo, 0x00000000);
      todo = *area;
      }
    }
  if((area->x + area->w) > 800) {
    SDL_SetClipRect(screen, &todo);
    SDL_FillRect(screen, &todo, 0x00000000);
    todo = *area;
    memset(mo, -1, sizeof(mo));
    update_buttons();
    SDL_SetClipRect(screen, NULL);
    }
  SDL_UpdateRects(screen, 1, area);
  }

void page_draw() {
  SDL_FillRect(screen, NULL, 0);

  if(page == PAGE_ROOT) SDL_BlitSurface(intro, NULL, screen, NULL);
  if(page == PAGE_NEW) cur_game->Clear();
  if(page == PAGE_GALAXY) {
    cur_game->Fill();
    SDL_Rect rec = {0, 0, 3, 3};
    for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
      rec.x = cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - 1;
      rec.y = cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - 1;
      SDL_FillRect(screen, &rec, 0xFFFFFFFF);
      }
    }
  if(page == PAGE_SYSTEM) {
    SDL_Rect destr = {(768-32)/2, (768-32)/2, 32, 32};
    SDL_BlitSurface(star, NULL, screen, &destr);
    destr.w = 3;  destr.h = 3;
    System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
    for(int plan=0; plan < sys->num_planets; ++plan) {
      destr.x = sys->planets[plan]->XPos(cur_game->turn) - 1;
      destr.y = sys->planets[plan]->YPos(cur_game->turn) - 1;
      SDL_FillRect(screen, &destr, 0xFFFF00FF);
      SDL_UpdateRects(screen, 1, &destr);
      }
    }
  if(page == PAGE_PLANET) {
    SDL_Rect destr = {0, 0, 768, 768};
    System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
    int ptype = sys->planets[cur_planet]->type;
    SDL_BlitSurface(planet[ptype], NULL, screen, &destr);
    page_update();
    }
  memset(mo, -1, sizeof(mo));
  update_buttons();

  SDL_UpdateRect(screen, 0, 0, 0, 0);
  }

void page_update() {
  static int lasttick = -1;
  if(page == PAGE_PLANET) {
    SDL_Rect srcr = {0, 0, 3, 3};
    SDL_Rect destr = {0, 0, 3, 3};
    Planet *plan = cur_game->galaxys[cur_galaxy]->systems[cur_system]->planets[cur_planet];
    int ptype = plan->type;
    if(lasttick != -1) {
      for(int sat=0; sat < plan->num_satellites; ++sat) {
	srcr.x = plan->satellites[sat]->XPos(lasttick) - 1;
	srcr.y = plan->satellites[sat]->YPos(lasttick) - 1;
	destr.x = plan->satellites[sat]->XPos(lasttick) - 1;
	destr.y = plan->satellites[sat]->YPos(lasttick) - 1;
	SDL_FillRect(screen, &destr, 0x00000000);
	SDL_BlitSurface(planet[ptype], &srcr, screen, &destr);
	if(overlaps(destr, mouser)) cursor_draw();
	SDL_UpdateRects(screen, 1, &destr);
	}
      }
    for(int sat=0; sat < plan->num_satellites; ++sat) {
      srcr.x = plan->satellites[sat]->XPos(cur_game->tick) - 1;
      srcr.y = plan->satellites[sat]->YPos(cur_game->tick) - 1;
      destr.x = plan->satellites[sat]->XPos(cur_game->tick) - 1;
      destr.y = plan->satellites[sat]->YPos(cur_game->tick) - 1;
      SDL_FillRect(screen, &destr, 0xFFFFFF00);
      if(plan->satellites[sat]->InFront(cur_game->tick))
	SDL_BlitSurface(planet[ptype], &srcr, screen, &destr);
      if(overlaps(destr, mouser)) cursor_draw();
      SDL_UpdateRects(screen, 1, &destr);
      }
    }
  lasttick = cur_game->tick;
  }

void page_clicked(int mx, int my, int mb) {
  if(page == PAGE_GALAXY) {
    int offx, offy;
    for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
      offx = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->xpos - mx);
      offy = abs(cur_game->galaxys[cur_galaxy]->systems[sys]->ypos - my);
      if(offx < 4 && offy < 4) {
	cur_system = sys;
	page = PAGE_SYSTEM;
	break;
	}
      }
    }
  else if(page == PAGE_SYSTEM) {
    int offx, offy;
    System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
    for(int plan=0; plan < sys->num_planets; ++plan) {
      offx = abs(mousex - sys->planets[plan]->XPos(cur_game->turn));
      offy = abs(mousey - sys->planets[plan]->YPos(cur_game->turn));
      if(offx < 4 && offy < 4) {
	cur_planet = plan;
	page = PAGE_PLANET;
	break;
	}
      }
    }
  }
