#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cctype>

#include <SDL.h>

#include "data/font22_black.h"
#include "data/font22_blue.h"
#include "data/font22_red.h"
#include "data/font22_green.h"
#include "data/font22_white.h"

static const unsigned char rchanc[] = { 0xFF, 0, 0, 0 };
static const unsigned int rchan = *((unsigned long *)rchanc);
static const unsigned char gchanc[] = { 0, 0xFF, 0, 0 };
static const unsigned int gchan = *((unsigned long *)gchanc);
static const unsigned char bchanc[] = { 0, 0, 0xFF, 0 };
static const unsigned int bchan = *((unsigned long *)bchanc);
static const unsigned char achanc[] = { 0, 0, 0, 0xFF };
static const unsigned int achan = *((unsigned long *)achanc);

struct font {
  SDL_Surface *img;
  int off[128];
  int len[128];
  };

font *font_init() {
  int xpos = 0;
  font *f = new font;
  for(int let=0; let<128; ++let) {
    if(let == ' ') {
      int pres = 0;
      f->off[let] = 0;
      while((!pres) && xpos < (int)font22_white_image.width) {
	pres = 0;
	for(int ctr=0; ctr < int(font22_white_image.height) && (!pres); ++ctr) {
	  if(font22_white_image.pixel_data[4*(font22_white_image.width*ctr+xpos)+3])
	    pres = 1;
	  }
	++xpos;
	}
      --xpos;
      f->len[let] = xpos-f->off[let];
      }
    else if(!isgraph(let)) { f->off[let] = xpos;  f->len[let] = 0; }
    else {
      int pres = 1;
      f->off[let] = xpos;
      while(pres && xpos < (int)font22_white_image.width) {
	pres = 0;
	for(int ctr=0; ctr < int(font22_white_image.height) && (!pres); ++ctr) {
	  if(font22_white_image.pixel_data[4*(font22_white_image.width*ctr+xpos)+3])
	    { pres = 1; }
	  }
	++xpos;
	}

      f->len[let] = xpos-f->off[let];
      while((!pres) && xpos < (int)font22_white_image.width) {
	pres = 0;
	for(int ctr=0; ctr < int(font22_white_image.height) && (!pres); ++ctr) {
	  if(font22_white_image.pixel_data[4*(font22_white_image.width*ctr+xpos)+3])
	    pres = 1;
	  }
	++xpos;
	}
      --xpos;
      }
    }

  SDL_Surface *tmp = SDL_CreateRGBSurfaceFrom(
		(void*)font22_white_image.pixel_data,
		font22_white_image.width, font22_white_image.height,
		32, 4*font22_white_image.width,
		rchan, gchan, bchan, achan);
  f->img = SDL_DisplayFormatAlpha(tmp);
  SDL_FreeSurface(tmp);
  SDL_SetAlpha(f->img, SDL_RLEACCEL|SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
  return f;
  }

int string_len(const char *str, font *f) {
  int ret=0;
  for(int ctr=0; ctr<int(strlen(str)); ++ctr) {
    ret += f->len[str[ctr]];
    }
  return ret;
  }

void string_draw(SDL_Surface *s, int x, int y, font *f, const char *t) {
  SDL_Rect sr, dr;
  int xpos = x;
  sr.y = 0;
  sr.h = 24;
  dr.y = y;

  for(int ctr=0; ctr<int(strlen(t)); ++ctr) {
    if(isgraph(t[ctr]) || t[ctr] == ' ') {
      sr.x = f->off[t[ctr]];
      sr.w = f->len[t[ctr]];
      dr.x = xpos;
      SDL_BlitSurface(f->img, &sr, s, &dr);
      xpos += f->len[t[ctr]];
      }
    else if(t[ctr] == '\n') {
      xpos = x;
      dr.y += 24;
      }
    }
  }

void string_drawr(SDL_Surface *s, int x, int y, font *f, const char *t) {
  x -= string_len(t, f);
  string_draw(s, x, y, f, t);
  }

font *font_colored(font *f, unsigned long c) {
  font *ret = new font;
  (*ret) = (*f);
  ret->img = SDL_DisplayFormatAlpha(f->img);
  SDL_LockSurface(ret->img);
//  c = SDL_MapRGBA(ret->img->format,
//	(c>>24)&0xFF, (c>>16)&0xFF, (c>>8)&0xFF, (c)&0xFF);
  if(ret->img->format->BytesPerPixel != 4) {
    fprintf(stderr, "Error: You need to be in 32-bit mode\n");
    exit(0);
    }
  unsigned long *tmp = (unsigned long *)(ret->img->pixels);
  int base=0;
  for(int y=0; y<ret->img->h; ++y) {
    for(int x=0; x<ret->img->w; ++x) {
      tmp[base+x] &= c;
      }
    base += ret->img->pitch/4;
    }
  SDL_UnlockSurface(ret->img);
  return ret;
  }
