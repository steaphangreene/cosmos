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

unsigned long black;

int done = 0;

extern SDL_Surface *screen;
font *cur_font[9];

SDL_Rect mouser = {-1, -1, 0, 0};
SDL_Surface *cursor, *button[BUTTON_MAX][2];

static SDL_Surface *intro;
static SDL_Surface *planet[4], *satellite[4];

int lastpage = PAGE_INVALID, page = PAGE_ROOT;
int buttlist[PAGE_MAX][BUTTON_MAX] = {{0}};
int pagemap[PAGE_MAX][BUTTON_MAX] = {{0}};

int mousex=0, mousey=0, mouseb=0;
int mo[BUTTON_MAX] = {0};

int ambient[PAGE_MAX] = {0}, click1, click2;
Sound *cur_amb=NULL;
int cur_amb_num=0;
int cur_galaxy=0;
int cur_system=0;
int cur_planet=0;

SDL_Surface *build_button0(const char *);
SDL_Surface *build_button1(const char *);
void gui_button_clicked(int button);
void cursor_draw();
void page_clicked(int mx, int my, int mb);
void page_draw();
void page_update();
void page_redraw(SDL_Rect *);

static SDL_Surface *base0 = NULL;
static SDL_Surface *base1 = NULL;

SDL_Surface *build_button0(const char *label) { 
  if(!base0) base0 = get_blank0_image();
  SDL_Surface *s = SDL_DisplayFormat(base0);

  int len = string_len(label, cur_font[4]);
  string_draw(s, 100-(len/2), 13, cur_font[4], label);
  return s;
  }

SDL_Surface *build_button1(const char *label) {
  if(!base1) base1 = get_blank1_image();
  SDL_Surface *s = SDL_DisplayFormat(base1);

  int len = string_len(label, cur_font[4]);
  string_draw(s, 100-(len/2), 13, cur_font[4], label);
  return s;
  }

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
      update(812, 64*ord, 200, 50);
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
    do_updates();
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
	  toggle_fullscreen();
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
	audio_play(click1, 8, 8);
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
  cur_font[8] = font_init();
  cur_font[7] = font_colored(cur_font[8], color3(7));
  cur_font[6] = font_colored(cur_font[8], color3(6));
  cur_font[5] = font_colored(cur_font[8], color3(5));
  cur_font[4] = font_colored(cur_font[8], color3(4));
  cur_font[3] = font_colored(cur_font[8], color3(3));
  cur_font[2] = font_colored(cur_font[8], color3(2));
  cur_font[1] = font_colored(cur_font[8], color3(1));
  cur_font[0] = font_colored(cur_font[8], color3(0));

  black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  cursor = get_cursor_image();
  intro = get_image("graphics/intro.raw", 800, 768);

  planet[0] = get_alpha_image("graphics/planet00.raw", 768, 768);
  satellite[0] = get_alpha_image("graphics/moon00.raw", 64, 64);
  satellite[1] = get_alpha_image("graphics/moon01.raw", 64, 64);
  satellite[2] = get_alpha_image("graphics/satellite00.raw", 8, 8);

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
  button[BUTTON_CLEARALL][0] = build_button0("Clear All");;
  button[BUTTON_CLEARALL][1] = build_button1("Clear All");
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

  buttlist[PAGE_NEW][BUTTON_CLEARALL] =		7;
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

  buttlist[PAGE_PLANET][BUTTON_EXIT] =		11;
  pagemap[PAGE_PLANET][BUTTON_EXIT] =		PAGE_SYSTEM;

  click1 = audio_loadsound("sounds/click01.wav");
  click2 = audio_loadsound("sounds/click02.wav");

  gui_init_galaxy();
  gui_init_system();
  page_init();
  }

void gui_button_clicked(int button) {
  switch(page) {
    case(PAGE_GALAXY): {
      gui_button_clicked_galaxy(button);
      } break;
    case(PAGE_SYSTEM): {
      gui_button_clicked_system(button);
      } break;
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
	  page_draw();
	  } break;
	case(BUTTON_CLEARALL): {
	  cur_game->ResetToUnknown();
	  page_draw();
	  } break;
	case(BUTTON_RESETALL): {
	  cur_game->Reset();
	  page_draw();
	  } break;
	case(BUTTON_CANCEL): {
	  cur_game->Reset();
	  } break;
	case(BUTTON_ACCEPT): {
	  cur_game->Clear();
	  cur_game->Fill();
	  page_draw();
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

  page_redraw(&mouser);
  SDL_BlitSurface(cursor, &cursorr, screen, &mouser);
  update(&mouser);
  }

void page_redraw(SDL_Rect *area) {
//  MUST NOT CALL "page_update" - MOUSE CURSOR LOOP!!!!
  SDL_Rect todo = *area;
  if(area->x < 800) {
    if(page == PAGE_ROOT) {
      SDL_BlitSurface(intro, &todo, screen, &todo);
      todo = *area;
      }
    else if(page == PAGE_NEW) {
      SDL_FillRect(screen, &todo, black);
      todo = *area;
      for(int set=0; set<num_configs; ++set) {
	int xp = 256-string_len(config[set][0], cur_font[4]);
	SDL_Rect lrec = {xp, 12+24*set, string_len(config[set][0], cur_font[4]), 24};
	SDL_Rect rrec = {270, 12+24*set,
		string_len(config[set][cur_game->working_setting[set]+1], cur_font[4]),
		24};

	if(overlaps(lrec, todo))
	  string_draw(screen, xp, 12+24*set, cur_font[4], config[set][0]);
	if(overlaps(rrec, todo)) {
	  if(config[set][cur_game->working_setting[set]+1][0] != '@') {
	    string_draw(screen, 270, 12+24*set, cur_font[4],
		config[set][cur_game->working_setting[set]+1]);
	    }
	  else {
	    string_draw(screen, 270, 12+24*set,
		cur_font[cur_game->working_setting[set]+1],
		config[set][cur_game->working_setting[set]+1]+1);
	    }
	  }
	}
      }
    else if(page == PAGE_GALAXY) {
      page_redraw_galaxy(area);
      }
    else if(page == PAGE_SYSTEM) {
      page_redraw_system(area);
      }
    else if(page == PAGE_PLANET) {
      SDL_FillRect(screen, &todo, black);
      System *sys = cur_game->galaxys[cur_galaxy]->systems[cur_system];
      Planet *plan = sys->planets[cur_planet];
      SDL_BlitSurface(planet[plan->type], &todo, screen, &todo);

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
	    SDL_BlitSurface(planet[plan->type], &srcr, screen, &destr);
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
      }
    else {
      SDL_FillRect(screen, &todo, black);
      todo = *area;
      }
    }
  if((area->x + area->w) > 800) {
    SDL_SetClipRect(screen, &todo);
    SDL_FillRect(screen, &todo, black);
    todo = *area;
    memset(mo, -1, sizeof(mo));
    update_buttons();
    SDL_SetClipRect(screen, NULL);
    }
  update(area);
  }

void page_draw() {
  SDL_FillRect(screen, NULL, 0);

  if(page == PAGE_ROOT) SDL_BlitSurface(intro, NULL, screen, NULL);
  if(page == PAGE_NEW) {
    for(int set=0; set<num_configs; ++set) {
      int xp = 256-string_len(config[set][0], cur_font[4]);
      string_draw(screen, xp, 12+24*set, cur_font[4], config[set][0]);
      if(config[set][cur_game->working_setting[set]+1][0] != '@') {
	string_draw(screen, 270, 12+24*set, cur_font[4],
		config[set][cur_game->working_setting[set]+1]);
	}
      else {
	string_draw(screen, 270, 12+24*set,
		cur_font[cur_game->working_setting[set]+1],
		config[set][cur_game->working_setting[set]+1]+1);
	}
      }
    }
  if(page == PAGE_GALAXY) {
    page_draw_galaxy();
    }
  if(page == PAGE_SYSTEM) {
    page_draw_system();
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

  update_all();
  }

void page_update() {
  static int lasttick = -1;
  if(page == PAGE_PLANET) {
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
	SDL_BlitSurface(planet[plan->type], &srcr, screen, &destr);
	if(overlaps(destr, mouser)) cursor_draw();
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
	SDL_BlitSurface(planet[plan->type], &srcr, screen, &destr);
	if(overlaps(destr, mouser)) cursor_draw();
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
	if(overlaps(destr, mouser)) cursor_draw();
	update(&destr);
	}
      }
    }
  else if(page == PAGE_GALAXY) {
    page_update_galaxy();
    }
  else if(page == PAGE_SYSTEM) {
    page_update_system();
    }
  lasttick = cur_game->tick;
  }

void page_clicked(int mx, int my, int mb) {
  if(page == PAGE_NEW) {
    if(mx < 512 && my >= 12 && my < 12+24*num_configs && (mb==1 || mb==3)) {
      int set = (my - 12)/24;
      if(mb == 1) {
	++cur_game->working_setting[set];
	if(cur_game->working_setting[set] >= num_options[set])
	  cur_game->working_setting[set] = 0;
	}
      else {
	--cur_game->working_setting[set];
	if(cur_game->working_setting[set] < 0)
	  cur_game->working_setting[set] = num_options[set]-1;
	}
      SDL_Rect r = {0, 12+24*set, 512, 24};
      audio_play(click2, 8, 8);
      page_redraw(&r);
      cursor_draw();
      }
    }
  else if(page == PAGE_GALAXY) {
    page_clicked_galaxy(mx, my, mb);
    }
  else if(page == PAGE_SYSTEM) {
    page_clicked_system(mx, my, mb);
    }
  }
