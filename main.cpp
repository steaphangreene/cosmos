#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

#include <SDL.h>

#include "gui.h"
#include "game.h"
#include "fonts.h"
#include "audio.h"

SDL_Surface *screen;

int use_sound = 1;

int main(int argc, char **argv) {
  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)) {
    fprintf(stderr, "Error initializing SDL!\n");
    exit(1);
    }

  Uint32 flags = SDL_HWSURFACE|SDL_FULLSCREEN;
  for(int ctr=1; ctr<argc; ++ctr) {
    if(!strcmp(argv[ctr], "--window")) flags &= (~(SDL_FULLSCREEN));
    else if(!strcmp(argv[ctr], "--nosound")) use_sound = 0;
    }

  screen = SDL_SetVideoMode(1024, 768, 32, flags);
  SDL_WM_SetCaption("Cosmos", "Cosmos");

  if(use_sound) audio_init();
  game_init();
  fonts_init();
  gui_init();
  gui_main();

  exit(0);
  return 0;
  }
