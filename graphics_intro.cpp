#include <SDL.h>

#include "intro.h"

SDL_Surface *get_intro_image() {
  return SDL_CreateRGBSurfaceFrom((void*)intro_image.pixel_data,
	intro_image.width, intro_image.height,
	32, 4*intro_image.width,
	0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
  }
