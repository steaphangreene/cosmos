#include <fcntl.h>

#include <SDL.h>

#include "graphics.h"
#include "data/blank0.h"
#include "data/blank1.h"
#include "data/star.h"

#include "fonts.h"

const unsigned char rchanc[] = { 0xFF, 0, 0, 0 };
const unsigned int rchan = *((unsigned long *)rchanc);
const unsigned char gchanc[] = { 0, 0xFF, 0, 0 };
const unsigned int gchan = *((unsigned long *)gchanc);
const unsigned char bchanc[] = { 0, 0, 0xFF, 0 };
const unsigned int bchan = *((unsigned long *)bchanc);
const unsigned char achanc[] = { 0, 0, 0, 0xFF };
const unsigned int achan = *((unsigned long *)achanc);

SDL_Surface *get_blank0_image() {
  return SDL_CreateRGBSurfaceFrom((void*)blank0_image.pixel_data,
	blank0_image.width, blank0_image.height,
	32, 4*blank0_image.width,
	rchan, gchan, bchan, achan);
  }

SDL_Surface *get_blank1_image() {
  return SDL_CreateRGBSurfaceFrom((void*)blank1_image.pixel_data,
	blank1_image.width, blank1_image.height,
	32, 4*blank1_image.width,
	rchan, gchan, bchan, achan);
  }

SDL_Surface *get_star_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)star_image.pixel_data,
	star_image.width, star_image.height,
	32, 4*star_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  return optim;
  }

SDL_Surface *get_intro_image() {
  FILE *gfl = fopen("graphics/intro.raw", "r");
  if(!gfl) {
    fprintf(stderr, "Warning: can't open \"graphics/intro.raw\"\n");
    return NULL;
    }
  unsigned char *img = new unsigned char[768*800*4];
  fread(img, 800*4, 768, gfl);

  SDL_Surface *s = SDL_CreateRGBSurfaceFrom(img, 800, 768, 32, 4*800,
				bchan, gchan, rchan, achan);
  SDL_Surface *ret = SDL_DisplayFormat(s);
  SDL_FreeSurface(s);
  return ret;
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
