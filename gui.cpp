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
SDL_Surface *intro, *button[BUTTON_MAX][2];
SDL_Event event;
SDL_Rect rect;

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

void gui_button_pressed(int button);
void draw_page();

int update_buttons() {
  int inbutt = 0;
  mouseb = SDL_GetMouseState(&mousex, &mousey);
  for(int ctr=1; ctr < BUTTON_MAX; ++ctr) if(buttlist[page][ctr]) {
    int ord = buttlist[page][ctr];
    int m = (mousey >= 64*ord && mousex >= 812
	&& mousey < 50+64*ord && mousex < 1012 && !(mouseb));
    if(m != mo[ctr]) {
      mo[ctr] = m;
      rect.x = 812;  rect.y = 64*ord;
      SDL_BlitSurface(button[ctr][mo[ctr]], NULL, screen, &rect);
      SDL_UpdateRect(screen, 812, 64*ord, 200, 50);
      }
    if(mousey >= 64*ord && mousex >= 812 && mousey < 50+64*ord && mousex < 1012)
      inbutt = ctr;
    }
  return inbutt;
  }

void page_init() {
  SDL_FillRect(screen, NULL, 0);

  draw_page();

  buttlist[PAGE_ROOT][BUTTON_RESUMEGAME] = (cur_game->InProgress())? 6 : 0;

  if(ambient[page] && ambient[page] != cur_amb_num &&
	ambient[page] != ambient[lastpage]) {
    audio_stop(cur_amb);
    cur_amb = audio_loop(ambient[page], 8, 0);
    cur_amb_num = ambient[page];
    }

  for(int ctr=1; ctr < BUTTON_MAX; ++ctr) if(buttlist[page][ctr]) {
    int ord = buttlist[page][ctr];
    mo[ctr] = (mousey >= 64*ord && mousex >= 812
		&& mousey < 50+64*ord && mousex < 1012);
    rect.x = 812;  rect.y = 64*ord;
    SDL_BlitSurface(button[ctr][0], NULL, screen, &rect);
    }
  memset(mo, 0, sizeof(mo));
  update_buttons();

  SDL_UpdateRect(screen, 0, 0, 0, 0);
  lastpage = page;
  }

void gui_main() {
  int curbutt=0;
  while(!done) {
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT) {
      done = 1;
      }
    else if(event.type == SDL_KEYDOWN) {
      if(event.key.keysym.sym == SDLK_ESCAPE) {
	done = 1;
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
      }
    else if(event.type == SDL_MOUSEBUTTONDOWN) {
      curbutt = update_buttons();
      if(curbutt) {
	audio_play(click, 8, 8);
	gui_button_pressed(curbutt);
	}
      }
    else if(event.type == SDL_MOUSEBUTTONUP) {
      if(curbutt == update_buttons() && curbutt && pagemap[page][curbutt]) {
	if(pagemap[page][curbutt] == -1) done = 1;
	else page = pagemap[page][curbutt];
	}
      }

    if(page != lastpage) {
      page_init();
      }
    }
  }

void gui_init() {
  intro = SDL_DisplayFormat(get_intro_image());

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
  ambient[PAGE_SYSOPT] = audio_loadsound("sounds/ambient00.wav");

  buttlist[PAGE_GALAXY][BUTTON_QUITGAME] =	11;
  pagemap[PAGE_GALAXY][BUTTON_QUITGAME] =	PAGE_ROOT;
  ambient[PAGE_GALAXY] = audio_loadsound("sounds/ambient00.wav");

  click = audio_loadsound("sounds/click.wav");

  page_init();
  }

void gui_button_pressed(int button) {
  switch(page) {
    case(PAGE_ROOT): {
      switch(button) {
	case(BUTTON_QUITGAME): {
	  SDL_Delay(500);
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

void draw_page() {
  if(page == PAGE_ROOT) SDL_BlitSurface(intro, NULL, screen, NULL);
  if(page == PAGE_NEW) cur_game->Clear();
  if(page == PAGE_GALAXY) {
    cur_game->Fill();
    SDL_Rect rec = {0, 0, 2, 2};
    for(int sys=0; sys < cur_game->galaxys[cur_galaxy]->num_systems; ++sys) {
      rec.x = cur_game->galaxys[cur_galaxy]->systems[sys]->xpos;
      rec.y = cur_game->galaxys[cur_galaxy]->systems[sys]->ypos;
      SDL_FillRect(screen, &rec, 0xFFFFFFFF);
      }
    }
  }
