#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cctype>

#include <SDL.h>

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

int string_length(const char *str, font *f) {
  int ret=0, ln=0;
  for(int ctr=0; ctr<int(strlen(str)); ++ctr) {
    if(str[ctr] == '\r') { ret = ret >? ln; ln = 0; }
    if(str[ctr] == '\t') { ln += 64; ln &= (~(63)); }
    else { ln += f->len[int(str[ctr])]; }
    }
  return ret >? ln;
  }

int string_height(const char *str, font *f) {
  int ret=22;
  for(int ctr=0; ctr<int(strlen(str)); ++ctr) {
    if(str[ctr] == '\n') ret += 24;
    }
  return ret;
  }

SDL_Surface *get_string(font *f, const char *t) {
  SDL_Rect sr, dr;
  int xpos = 0;
  sr.y = 0;
  sr.h = 24;
  dr.y = 0;

  int xs = string_length(t, f);
  int ys = string_height(t, f);

  SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,
	xs, ys, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
  SDL_FillRect(tmp, NULL, 0);

  SDL_SetAlpha(f->img, 0, 255);
  for(int ctr=0; ctr<int(strlen(t)); ++ctr) {
    if(isgraph(t[ctr]) || t[ctr] == ' ') {
      sr.x = f->off[int(t[ctr])];
      sr.w = f->len[int(t[ctr])];
      dr.x = xpos;
      SDL_BlitSurface(f->img, &sr, tmp, &dr);
      xpos += f->len[int(t[ctr])];
      }
    else if(t[ctr] == '\n') {
      xpos = 0;
      dr.y += 24;
      }
    else if(t[ctr] == '\t') {
      if(t[ctr] == '\t') { xpos += 64; xpos &= (~(63)); }
      }
    }
  SDL_SetAlpha(f->img, SDL_SRCALPHA|SDL_RLEACCEL, 255);

  SDL_Surface *ret = SDL_DisplayFormatAlpha(tmp);
  SDL_FreeSurface(tmp);
  SDL_SetAlpha(ret, SDL_SRCALPHA|SDL_RLEACCEL, 255);
  return ret;
  }

void string_draw(SDL_Surface *s, int x, int y, font *f, const char *t) {
  SDL_Rect fr = {x, y, 0, 0};
  SDL_Surface *mes = get_string(f, t);
  SDL_BlitSurface(mes, NULL, s, &fr);
  SDL_FreeSurface(mes);
  }

void string_drawc(SDL_Surface *s, int x, int y, font *f, const char *t) {
  x -= string_length(t, f)/2;
  string_draw(s, x, y, f, t);
  }

void string_drawr(SDL_Surface *s, int x, int y, font *f, const char *t) {
  x -= string_length(t, f);
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

font *font_backed(font *f, unsigned long c) {
  font *ret = new font;
  (*ret) = (*f);
  ret->img = SDL_DisplayFormat(f->img);
  SDL_FillRect(ret->img, NULL, 0);
  SDL_BlitSurface(f->img, NULL, ret->img, NULL);
  return ret;
  }
