#include <cstdio>
#include <cstdlib>
#include <cstdarg>
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

#include "data/click01.h"
#include "data/click02.h"

unsigned long black;

int done = 0;

#define	NUM_SYSCNF 1
static int syscnf[NUM_SYSCNF*2] = {1, 160};
static int syscnf_back[NUM_SYSCNF*2];

extern SDL_Surface *screen;
font *cur_font[9];

SDL_Surface *button[BUTTON_MAX][2];

static SDL_Surface *intro;

int lastpage = PAGE_INVALID, page = PAGE_ROOT;
int lastpanel = PANEL_NONE, panel = PANEL_NONE;
int buttlist[PAGE_MAX][BUTTON_MAX] = {{0}};
int pagemap[PAGE_MAX][BUTTON_MAX] = {{0}};

int mousex=0, mousey=0, mouseb=0;
int mo[BUTTON_MAX] = {0};
static int grabbed = -1;

int ambient[PAGE_MAX] = {0}, music[PAGE_MAX] = {0}, click1, click2;
Sound *cur_amb=NULL;
int cur_amb_num=0;
Sound *cur_mus=NULL;
int cur_mus_num=0;
int cur_galaxy=0;
int cur_system=0;
int cur_planet=0;

static char dialog_message[4096] = {0};

SDL_Surface *build_button0(const char *);
SDL_Surface *build_button1(const char *);
void button_clicked(int button);
void page_clicked(int mx, int my, int mb);
void panel_clicked(int mx, int my, int mb);
void mouse_moved(int mx, int my);
void mouse_released();
void page_draw();
void panel_draw();
void page_update();
void page_redraw(SDL_Rect *);
void panel_redraw(SDL_Rect *);

static SDL_Surface *base0 = NULL;
static SDL_Surface *base1 = NULL;
static SDL_Surface *check[2] = { NULL, NULL };

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
	&& mousey < 50+64*ord && mousex < 1012 && !(mouseb&1));
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

void panel_cleanup(int pan) {
  if(pan == PANEL_COLONY) {
    panel_cleanup_colony();
    }
  }

void panel_init() {
  if(panel == PANEL_COLONY) {
    panel_init_colony();
    }
  panel_draw();

  lastpanel = panel;
  }

void page_cleanup(int pag) {
  if(pag == PAGE_GALAXY) {
    page_cleanup_galaxy();
    }
  if(pag == PAGE_SYSTEM) {
    page_cleanup_system();
    }
  if(pag == PAGE_PLANET) {
    page_cleanup_planet();
    }
  if(pag == PAGE_SYSOPT) {
    memcpy(syscnf_back, syscnf, sizeof(syscnf));
    }
  }

void page_init() {
  if(page == PAGE_GALAXY) {
    page_init_galaxy();
    }
  if(page == PAGE_SYSTEM) {
    page_init_system();
    }
  if(page == PAGE_PLANET) {
    page_init_planet();
    }
  if(page == PAGE_SYSOPT) {
    memcpy(syscnf_back, syscnf, sizeof(syscnf));
    }
  page_draw();

  buttlist[PAGE_ROOT][BUTTON_RESUMEGAME] = (cur_game->InProgress())? 6 : 0;

  if(ambient[page] && ambient[page] != cur_amb_num &&
	ambient[page] != ambient[lastpage]) {
    audio_stop(cur_amb);
    cur_amb = audio_loop(ambient[page], 8, 0);
    cur_amb_num = ambient[page];
    }

  if(music[page] && music[page] != cur_mus_num &&
	music[page] != music[lastpage]) {
    audio_stop(cur_mus);
    cur_mus = audio_loop(music[page], syscnf[0] ? syscnf[1]/20 : 0, 0);
    cur_mus_num = music[page];
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
      else if(event.key.keysym.sym == SDLK_TAB) {
//	if(event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL)) {
	  if(cur_game) {
	    cur_game->TakeTurn();
	    page_init();
	    panel_init();
	    }
	  }
//	}
      }
    else if(event.type == SDL_MOUSEMOTION) {
      if(curbutt != update_buttons()) curbutt = 0;
      mouse_moved(mousex, mousey);
      }
    else if(event.type == SDL_MOUSEBUTTONDOWN) {
      if(event.button.button == 1) {
	curbutt = update_buttons();
	if(curbutt) {
	  audio_play(click1, 8, 8);
	  }
	else if(mousex < 768) {
	  page_clicked(mousex, mousey, event.button.button);
	  }
	else {
	  panel_clicked(mousex, mousey, event.button.button);
	  }
	}
      else if(mousex < 768) {
	page_clicked(mousex, mousey, event.button.button);
	}
      else {
	panel_clicked(mousex, mousey, event.button.button);
	}
      }
    else if(event.type == SDL_MOUSEBUTTONUP) {
      mouse_released();
      if(event.button.button == 1 && curbutt == update_buttons() && curbutt) {
	button_clicked(curbutt);
        if(pagemap[page][curbutt]) {
	  page = pagemap[page][curbutt];
	  }
	}
      }

    if(page != lastpage) {
      page_cleanup(lastpage);
      page_init();
      }

    if(panel != lastpanel) {
      page_cleanup(lastpanel);
      panel_init();
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

  grabbed = -1;

  check[0] = get_check0_image();
  check[1] = get_check1_image();

  black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  set_cursor(get_cursor_image());
  intro = get_image("graphics/intro.raw", 800, 768);

  button[BUTTON_RESUMEGAME][0] = build_button0("Resume Game");
  button[BUTTON_RESUMEGAME][1] = build_button1("Resume Game");
  button[BUTTON_LOADGAME][0] = build_button0("Load Game");
  button[BUTTON_LOADGAME][1] = build_button1("Load Game");
  button[BUTTON_NEWGAME][0] = build_button0("Create New Game");
  button[BUTTON_NEWGAME][1] = build_button1("Create New Game");
  button[BUTTON_NETWORKGAME][0] = build_button0("Network Game");
  button[BUTTON_NETWORKGAME][1] = build_button1("Network Game");
  button[BUTTON_SYSTEMOPTIONS][0] = build_button0("System Options");
  button[BUTTON_SYSTEMOPTIONS][1] = build_button1("System Options");
  button[BUTTON_QUITGAME][0] = build_button0("Quit Game");
  button[BUTTON_QUITGAME][1] = build_button1("Quit Game");
  button[BUTTON_CANCEL][0] = build_button0("Cancel");
  button[BUTTON_CANCEL][1] = build_button1("Cancel");
  button[BUTTON_ACCEPT][0] = build_button0("Accept");
  button[BUTTON_ACCEPT][1] = build_button1("Accept");
  button[BUTTON_OPTIONS][0] = build_button0("Options");
  button[BUTTON_OPTIONS][1] = build_button1("Options");
  button[BUTTON_EXIT][0] = build_button0("Exit");
  button[BUTTON_EXIT][1] = build_button1("Exit");
  button[BUTTON_TURN][0] = build_button0("Turn");
  button[BUTTON_TURN][1] = build_button1("Turn");
  button[BUTTON_RESETALL][0] = build_button0("Reset All");
  button[BUTTON_RESETALL][1] = build_button1("Reset All");
  button[BUTTON_CLEARALL][0] = build_button0("Clear All");
  button[BUTTON_CLEARALL][1] = build_button1("Clear All");
  button[BUTTON_RANDOMIZE][0] = build_button0("Randomize");
  button[BUTTON_RANDOMIZE][1] = build_button1("Randomize");
  button[BUTTON_NEWPROJECT][0] = build_button0("New Project");
  button[BUTTON_NEWPROJECT][1] = build_button1("New Project");
  button[BUTTON_CANCELPROJECT][0] = build_button0("Cancel Project");
  button[BUTTON_CANCELPROJECT][1] = build_button1("Cancel Project");
  button[BUTTON_CANCELBUILD][0] = build_button0("Cancel Build");
  button[BUTTON_CANCELBUILD][1] = build_button1("Cancel Build");
  button[BUTTON_BUILD][0] = build_button0("Build");
  button[BUTTON_BUILD][1] = build_button1("Build");
  button[BUTTON_ABANDON][0] = build_button0("Abandon");
  button[BUTTON_ABANDON][1] = build_button1("Abandon");

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
//  ambient[PAGE_ROOT] = audio_loadsound("sounds/ambient00.wav");
  music[PAGE_ROOT] = audio_loadmusic("sounds/music00.wav");

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

  buttlist[PAGE_DIALOG][BUTTON_EXIT] =		11;

  music[PAGE_GALAXY] = audio_loadmusic("sounds/music01.wav");

  click1 = audio_buildsound(click01, sizeof(click01));
  click2 = audio_buildsound(click02, sizeof(click02));

  gui_init_galaxy();
  gui_init_system();
  gui_init_planet();

  gui_init_colony();

  page_init();
  panel_init();
  }

void button_clicked(int button) {
  switch(panel) {
    case(PANEL_COLONY): {
      button_clicked_colony(button);
      } break;
    }
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
    case(PAGE_SYSOPT): {
      if(button == BUTTON_CANCEL) {
	memcpy(syscnf, syscnf_back, sizeof(syscnf));
	audio_setvol(cur_mus, syscnf[0] ? syscnf[1]/20 : 0);
	}
      }
    }
  }

void panel_redraw(SDL_Rect *area) {
  SDL_Rect todo = {0, 0, 1024, 768};
  if(area) todo = *area;
  SDL_SetClipRect(screen, &todo);
  SDL_FillRect(screen, &todo, black);
  if(area) todo = *area;

  if(panel == PANEL_COLONY) {
    panel_redraw_colony(area);
    }

  memset(mo, -1, sizeof(mo));
  update_buttons();
  }

void page_redraw(SDL_Rect *area) {
  SDL_Rect todo = {0, 0, 1024, 768};
  if(area) todo = *area;

  if((!area) || area->x < 800) {
    if(page == PAGE_ROOT) {
      if(intro) SDL_BlitSurface(intro, &todo, screen, &todo);
      else SDL_FillRect(screen, &todo, black);
      if(area) todo = *area;
      }
    else if(page == PAGE_NEW) {
      SDL_FillRect(screen, &todo, black);
      if(area) todo = *area;
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
      page_redraw_planet(area);
      }
    else if(page == PAGE_SYSOPT) {
      SDL_Rect pos = {8, 13, 22, 22};
      SDL_BlitSurface(check[syscnf[0]], NULL, screen, &pos);
      SDL_Rect bar = {368-8, 13, syscnf[1], 22};
      SDL_FillRect(screen, &bar, SDL_MapRGB(screen->format, 0xFF,0x00,0x00));
      bar.x += bar.w;
      bar.w = 400 - bar.w;
      SDL_FillRect(screen, &bar, SDL_MapRGB(screen->format, 0x7F,0x00,0x00));
      }
    else {
      SDL_FillRect(screen, &todo, black);
      if(area) todo = *area;
      }
    }
  if((area) && (area->x + area->w) > 800) {
    panel_redraw(area);
    }
  if(area) update(area);
  }

void panel_draw() {
  SDL_Rect pgr = {800, 0, 224, 768};
  SDL_FillRect(screen, &pgr, 0);
  if(panel == PANEL_COLONY) {
    panel_draw_colony();
    }
  memset(mo, -1, sizeof(mo));
  update_buttons();
  }

void page_draw() {
  SDL_Rect pgr = {0, 0, 800, 768};
  SDL_FillRect(screen, &pgr, 0);

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
  if(page == PAGE_SYSOPT) {
    string_draw(screen, 40, 13, cur_font[4], "Play Music");
    string_drawr(screen, 368-8-8, 13, cur_font[4], "Music Volume");
    page_redraw(NULL);
    }
  if(page == PAGE_DIALOG) {
    string_draw(screen, 8, 13, cur_font[4], dialog_message);
    }
  if(page == PAGE_GALAXY) {
    page_draw_galaxy();
    }
  if(page == PAGE_SYSTEM) {
    page_draw_system();
    }
  if(page == PAGE_PLANET) {
    page_draw_planet();
    }
  update_all();
  }

void page_update() {
  if(page == PAGE_PLANET) {
    page_update_planet();
    }
  else if(page == PAGE_GALAXY) {
    page_update_galaxy();
    }
  else if(page == PAGE_SYSTEM) {
    page_update_system();
    }
  if(panel == PANEL_COLONY) {
    panel_update_colony();
    }
  }

void mouse_released() {
  if(page == PAGE_GALAXY) {
    mouse_released_galaxy();
    }
  else if(page == PAGE_SYSTEM) {
    mouse_released_system();
    }
  else if(page == PAGE_PLANET) {
    mouse_released_planet();
    }
  else if(page == PAGE_SYSOPT) {
    grabbed = -1;
    }
  if(panel == PANEL_COLONY) {
    mouse_released_colony();
    }
  }

void mouse_moved(int mx, int my) {
  if(page == PAGE_GALAXY) {
    mouse_moved_galaxy(mx, my);
    }
  else if(page == PAGE_SYSTEM) {
    mouse_moved_system(mx, my);
    }
  else if(page == PAGE_PLANET) {
    mouse_moved_planet(mx, my);
    }
  else if(page == PAGE_SYSOPT) {
    if(grabbed != -1) {
      page_clicked(360 >? mx <? 759, 24 + grabbed*24, 1);
      }
    }
  if(panel == PANEL_COLONY) {
    mouse_moved_colony(mx, my);
    }
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
      }
    }
  else if(page == PAGE_SYSOPT) {
    if(mx >= 360 && mx < 760) {
      SDL_Rect bar = {360, 0, 400, 22};
      my -= 13;
      if(my >= 0 && (my % 24) < 22 && (my / 24) < NUM_SYSCNF) {
	grabbed = my/24;
	bar.y = 13+grabbed*24;
	syscnf[2*grabbed+1] = mx-360;
	page_redraw(&bar);
	if(grabbed == 0) {
	  if(syscnf[0]) audio_setvol(cur_mus, syscnf[1]/20);
	  }
	}
      }
    else if(mx >= 8 && mx < 32) {
      SDL_Rect chk = {8, 0, 24, 22};
      my -= 13;
      if(my >= 0 && (my % 24) < 22 && (my / 24) < NUM_SYSCNF) {
	chk.y = 13+(my/24)*24;
	syscnf[2*(my/24)] = !syscnf[2*(my/24)];
	audio_play(click2, 8, 8);
	page_redraw(&chk);
	if((my/24) == 0) {
	  if(!syscnf[0]) audio_setvol(cur_mus, 0);
	  else audio_setvol(cur_mus, syscnf[1]/20);
	  }
	}
      }
    }
  else if(page == PAGE_DIALOG) {
    page = pagemap[page][BUTTON_EXIT];
    }
  else if(page == PAGE_GALAXY) {
    page_clicked_galaxy(mx, my, mb);
    }
  else if(page == PAGE_SYSTEM) {
    page_clicked_system(mx, my, mb);
    }
  else if(page == PAGE_PLANET) {
    page_clicked_planet(mx, my, mb);
    }
  }

void panel_clicked(int mx, int my, int mb) {
  if(panel == PANEL_COLONY) {
    panel_clicked_colony(mx, my, mb);
    }
  }

void do_dialog(const char *fmt, ...) {
  va_list stuff;
  va_start(stuff, fmt);
  vsprintf(dialog_message, fmt, stuff);
  va_end(stuff);
  if(page != PAGE_DIALOG) pagemap[PAGE_DIALOG][BUTTON_EXIT] = page;
  page = PAGE_DIALOG;
  page_init();
  }
