#include <SDL.h>

#include "blank0.h"
#include "blank1.h"

#include "fonts.h"

SDL_Surface *get_blank0_image() {
  return SDL_CreateRGBSurfaceFrom((void*)blank0_image.pixel_data,
	blank0_image.width, blank0_image.height,
	32, 4*blank0_image.width,
	0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
  }

SDL_Surface *get_blank1_image() {
  return SDL_CreateRGBSurfaceFrom((void*)blank1_image.pixel_data,
	blank1_image.width, blank1_image.height,
	32, 4*blank1_image.width,
	0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
  }

static SDL_Surface *base0 = NULL;
static SDL_Surface *base1 = NULL;

SDL_Surface *build_button0(const char *label) {
  if(!base0) base0 = get_blank0_image();
  SDL_Surface *s = SDL_DisplayFormat(base0);

  int len = string_len(label);
  string_draw(s, 100-(len/2), 13, 0x0000007F, label);
  return s;
  }

SDL_Surface *build_button1(const char *label) {
  if(!base1) base1 = get_blank1_image();
  SDL_Surface *s = SDL_DisplayFormat(base1);

  int len = string_len(label);
  string_draw(s, 100-(len/2), 13, 0x0000007F, label);
  return s;
  }
