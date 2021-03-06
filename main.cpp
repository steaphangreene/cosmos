#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

extern const char *version;

#include <SDL.h>

#include "gui.h"
#include "game.h"
#include "fonts.h"
#include "audio.h"
#include "graphics.h"

int use_sound = 1;

#ifndef DEBUG
#define DEBUG	0
#endif

int main(int argc, char **argv) {
  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER|DEBUG)) {
    fprintf(stderr, "Error initializing SDL!\n");
    exit(1);
    }

  Uint32 flags = SDL_HWSURFACE|SDL_FULLSCREEN;
  for(int ctr=1; ctr<argc; ++ctr) {
    if(!strcmp(argv[ctr], "--window")) flags &= (~(SDL_FULLSCREEN));
    else if(!strcmp(argv[ctr], "--nosound")) use_sound = 0;
    }

  graphics_init(1024, 768, 32, flags);
  SDL_WM_SetCaption("Cosmos", "Cosmos");

  if(use_sound) audio_init();
  game_init();
  SDL_ShowCursor(SDL_DISABLE);
  gui_init();
  cur_game->Load("autosave.gam");
  gui_main();
  SDL_ShowCursor(SDL_ENABLE);
  cur_game->Save("autosave.gam");
  SDL_Quit();

  exit(0);
  return 0;
  }
