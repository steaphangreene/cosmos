#include <fcntl.h>
#include <cstring>
//#include <unistd.h>
//#include <sys/mman.h>

#include <SDL.h>

#include "graphics.h"
#include "data/cursor.h"
#include "data/blank0.h"
#include "data/blank1.h"
#include "data/check0.h"
#include "data/check1.h"
#include "data/star00.h"
#include "data/gstar00.h"
#include "data/splanet00.h"

#include "fonts.h"

const unsigned char rchanc[] = { 0xFF, 0, 0, 0 };
const unsigned int rchan = *((unsigned long *)rchanc);
const unsigned char gchanc[] = { 0, 0xFF, 0, 0 };
const unsigned int gchan = *((unsigned long *)gchanc);
const unsigned char bchanc[] = { 0, 0, 0xFF, 0 };
const unsigned int bchan = *((unsigned long *)bchanc);
const unsigned char achanc[] = { 0, 0, 0, 0xFF };
const unsigned int achan = *((unsigned long *)achanc);

static SDL_Surface *cursor = NULL, *cursorb = NULL;
static SDL_Rect mouser = { -1, -1, 0, 0 };

SDL_Surface *get_blank0_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)blank0_image.pixel_data,
	blank0_image.width, blank0_image.height,
	32, 4*blank0_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_blank1_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)blank1_image.pixel_data,
	blank1_image.width, blank1_image.height,
	32, 4*blank1_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_check0_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)check0_image.pixel_data,
	check0_image.width, check0_image.height,
	32, 4*check0_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_check1_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)check1_image.pixel_data,
	check1_image.width, check1_image.height,
	32, 4*check1_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_cursor_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)cursor_image.pixel_data,
	cursor_image.width, cursor_image.height,
	32, 4*cursor_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_star_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)star00_image.pixel_data,
	star00_image.width, star00_image.height,
	32, 4*star00_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_gstar_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)gstar00_image.pixel_data,
	gstar00_image.width, gstar00_image.height,
	32, 4*gstar00_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_splanet_image() {
  SDL_Surface *orig = SDL_CreateRGBSurfaceFrom((void*)splanet00_image.pixel_data,
	splanet00_image.width, splanet00_image.height,
	32, 4*splanet00_image.width,
	rchan, gchan, bchan, achan);
  SDL_Surface *optim = SDL_DisplayFormatAlpha(orig);
  SDL_FreeSurface(orig);
  SDL_SetAlpha(optim, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return optim;
  }

SDL_Surface *get_image(const char *filename, int xs, int ys) {
  FILE *gfl = fopen(filename, "r");
  if(!gfl) {
    fprintf(stderr, "Warning: can't open \"%s\"\n", filename);
    return NULL;
    }
  unsigned char *img;
//  img = (unsigned char *)mmap(NULL, xs*ys*4,
//	PROT_READ, MAP_PRIVATE, fileno(gfl), 0);
  img = new unsigned char[xs*ys*4];
  fread(img, xs*4, ys, gfl);  // Stupid Windows not letting me mmap()!

  SDL_Surface *s = SDL_CreateRGBSurfaceFrom(img, xs, ys, 32, 4*xs,
				bchan, gchan, rchan, achan);
  SDL_Surface *ret = SDL_DisplayFormat(s);
  SDL_FreeSurface(s);
  SDL_SetAlpha(ret, SDL_SRCALPHA|SDL_RLEACCEL, 0xFF);
  return ret;
  }

SDL_Surface *get_alpha_image(const char *filename, int xs, int ys) {
  FILE *gfl = fopen(filename, "r");
  if(!gfl) {
    fprintf(stderr, "Warning: can't open \"%s\"\n", filename);
    return NULL;
    }
  unsigned char *img = new unsigned char[xs*ys*4];
  fread(img, xs*4, ys, gfl);  // Stupid Windows not letting me mmap()!

  SDL_Surface *s = SDL_CreateRGBSurfaceFrom(img, xs, ys, 32, 4*xs,
				bchan, gchan, rchan, achan);
  SDL_Surface *ret = SDL_DisplayFormatAlpha(s);
  SDL_FreeSurface(s);
  return ret;
  }

//Double-buffering system
static int num_updaterecs = 0;
static SDL_Rect updaterecs[1024];

SDL_Surface *framebuffer, *screen;

void graphics_init(int w, int h, int bpp, Uint32 flags) {
  framebuffer = SDL_SetVideoMode(w, h, bpp, flags);
  screen = SDL_DisplayFormat(framebuffer);
  }

void update_all() {
  num_updaterecs = -1;
  }

void update(SDL_Rect *r) {
  if(num_updaterecs < 0) return;
  updaterecs[num_updaterecs] = *r;
  ++num_updaterecs;
  }

void update(int x, int y, unsigned int w, unsigned int h) {
  if(num_updaterecs < 0) return;
  updaterecs[num_updaterecs].x = x;
  updaterecs[num_updaterecs].y = y;
  updaterecs[num_updaterecs].w = w;
  updaterecs[num_updaterecs].h = h;
  ++num_updaterecs;
  }

void do_updates() {
  int x, y, moved = 0;
  static SDL_Rect cursorr;
  SDL_GetMouseState(&x, &y);
  if(x != mouser.x || y != mouser.y) {
    update(&mouser);
    moved = 1;
    mouser.x = x;
    mouser.y = y;
    }
  mouser.w = cursor->w;
  mouser.h = cursor->h;
  cursorr = mouser;
  cursorr.x = 0; cursorr.y = 0;
  SDL_BlitSurface(screen, &mouser, cursorb, &cursorr);
  SDL_BlitSurface(cursor, NULL, screen, &mouser);
  if(moved) update(&mouser);

  if(num_updaterecs < 0) {
    SDL_BlitSurface(screen, NULL, framebuffer, NULL);
    SDL_UpdateRect(framebuffer, 0, 0, 0, 0);
    }
  else {
    for(int ctr=0; ctr<num_updaterecs; ++ctr)
      SDL_BlitSurface(screen, updaterecs+ctr, framebuffer, updaterecs+ctr);
    SDL_UpdateRects(framebuffer, num_updaterecs, updaterecs);
    }
  num_updaterecs = 0;

  SDL_BlitSurface(cursorb, &cursorr, screen, &mouser);
  }

unsigned int color3(int c) {
  return (0xFF000000 |
	((c&4)? 0x00BF0000 : 0x000000) |
	((c&2)? 0x0000BF00 : 0x000000) |
	((c&1)? 0x000000BF : 0x000000)
	);
  }

void toggle_fullscreen() {
  char drv[16] = {0};
  SDL_VideoDriverName((char *)drv, 15);
  if(strcmp(drv, "x11")) return;
  SDL_WM_ToggleFullScreen(framebuffer);
  }

void set_cursor(SDL_Surface *c) {
  cursor = c;
  cursorb = SDL_DisplayFormat(cursor);
  }
