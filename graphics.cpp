#include <fcntl.h>
#include <cstring>
#include <cmath>
#include <vector>
//#include <unistd.h>
//#include <sys/mman.h>

using namespace std;

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

static vector<SDL_Surface *> sprite, spriteb;
static vector<SDL_Rect> spriter;
static vector<unsigned long> spritef;

#define SPRITE_UPDATE	1

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
  static SDL_Rect wholer = {0, 0, 0, 0};
  for(int ctr=0; ctr<int(sprite.size()); ++ctr) if(sprite[ctr]) {
    if(ctr == 0) {
      int x, y;
      SDL_GetMouseState(&x, &y);
      if(x != spriter[ctr].x || y != spriter[ctr].y) {
	update(&spriter[ctr]);
	spriter[ctr].x = x;
	spriter[ctr].y = y;
	spriter[ctr].w = sprite[ctr]->w;
	spriter[ctr].h = sprite[ctr]->h;
	update(&spriter[ctr]);
	}
      }
    else {
      spriter[ctr].w = sprite[ctr]->w;
      spriter[ctr].h = sprite[ctr]->h;
      if(spritef[ctr] & SPRITE_UPDATE) {
	update(&spriter[ctr]);
	spritef[ctr] &= (~(SPRITE_UPDATE));
	}
      }
    SDL_BlitSurface(screen, &spriter[ctr], spriteb[ctr], &wholer);
    }
  for(int ctr=int(sprite.size())-1; ctr >= 0; --ctr) if(sprite[ctr]) {
    SDL_BlitSurface(sprite[ctr], NULL, screen, &spriter[ctr]);
    }

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

  for(int ctr=0; ctr<int(sprite.size()); ++ctr) if(sprite[ctr]) {
    SDL_BlitSurface(spriteb[ctr], NULL, screen, &spriter[ctr]);
    }
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

void set_cursor(SDL_Surface *s) {
  set_sprite(0, s);
  }

void set_sprite(int n, SDL_Surface *c) {
  while(int(sprite.size()) < n+1) {
    sprite.push_back(NULL);
    spriteb.push_back(NULL);
    SDL_Rect nullr = {-1, -1, 0, 0};
    spriter.push_back(nullr);
    spritef.push_back(0);
    }
  if(sprite[n] && (spritef[n] & SPRITE_UPDATE)) {
    update(&spriter[n]);
    spritef[n] &= (~(SPRITE_UPDATE));
    }
  if(sprite[n]) delete sprite[n];
  sprite[n] = c;
  if(c == NULL) {
    spriteb[n] = NULL;
    }
  else {
    spriteb[n] = SDL_DisplayFormat(sprite[n]);
    SDL_SetAlpha(spriteb[n], 0, 255);
    }
  }

void update_sprite(int n) {
  if(n < int(spritef.size())) spritef[n] |= (SPRITE_UPDATE);
  }

void move_sprite(int n, int x, int y) {
  if(n < int(spritef.size()) && sprite[n] && (spritef[n] & SPRITE_UPDATE)) {
    update(&spriter[n]);
    spritef[n] &= (~(SPRITE_UPDATE));
    }
  spriter[n].x = x;
  spriter[n].y = y;
  }

SDL_Surface *getline(int x1, int y1, int x2, int y2, Uint32 col, Uint32 pat) {
  if(abs(x1-x2) == 0 || abs(y1-y2) == 0) return NULL;

  SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,
	abs(x1-x2)+1, abs(y1-y2)+1, 32,
	0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
  SDL_FillRect(tmp, NULL, 0);

  int rot = 0, slp = 1;
  if(abs(y1-y2) > abs(x1-x2)) rot = 1;
  int rng = (abs(y1-y2) >? abs(x1-x2));
  int div = (abs(y1-y2) <? abs(x1-x2));
  if((x1 < x2 && y2 < y1) || (x2 < x1 && y1 < y2)) slp = -1;

  for(int ind=0; ind <= rng; ++ind) {
    if(!(pat & (1<<(ind&31)))) continue;
    int bas = ind;
    int sub = (ind*div)/rng;
    if(slp == -1) sub = div-sub;
    Uint32 rem = ((ind*div)%rng) * 255 / rng;
    Uint32 c1 = (col & 0xFFFFFFFF) ^ (rem << 24);
    Uint32 c2 = (col & 0x00FFFFFF) ^ (rem << 24);
    SDL_LockSurface(tmp);
    if(!rot) {
      ((long*)(tmp->pixels))[tmp->pitch/4*sub+bas] = c1;
      if(rem) ((long*)(tmp->pixels))[tmp->pitch/4*(sub+slp)+bas] = c2;
      }
    else {
      ((long*)(tmp->pixels))[tmp->pitch/4*bas+sub] = c1;
      if(rem) ((long*)(tmp->pixels))[tmp->pitch/4*bas+(sub+slp)] = c2;
      }
    SDL_UnlockSurface(tmp);
    }
  SDL_Surface *ret = SDL_DisplayFormatAlpha(tmp);
  SDL_FreeSurface(tmp);
  return ret;
  }

void drawline(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 col, Uint32 pat) {
  SDL_Surface *tmp = getline(x1, y1, x2, y2, col, pat);
  SDL_Rect destr = {x1<?x2, y1<?y2, abs(x1-x2)+1, abs(y1-y2)+1};
  SDL_BlitSurface(tmp, NULL, s, &destr);
  SDL_FreeSurface(tmp);
  update(&destr);
  }
