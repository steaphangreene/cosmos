#include <cstring>
#include <cctype>

#include <SDL.h>

#include "font22.h"

SDL_Surface *font;
int foff[128];
int flen[128];

void fonts_init() {
  int xpos = 0;
  for(int let=0; let<128; ++let) {
    if(let == ' ') {
      int pres = 0;
      foff[let] = 0;
      while((!pres) && xpos < (int)font22_image.width) {
	pres = 0;
	for(int ctr=0; ctr < int(font22_image.height) && (!pres); ++ctr) {
	  if(font22_image.pixel_data[4*(font22_image.width*ctr+xpos)+3])
	    pres = 1;
	  }
	++xpos;
	}
      --xpos;
      flen[let] = xpos-foff[let];

      /* HACK! */
      flen[let] = 5;
      }
    else if(!isgraph(let)) { foff[let] = xpos;  flen[let] = 0; }
    else {
      int pres = 1;
      foff[let] = xpos;
      while(pres && xpos < (int)font22_image.width) {
	pres = 0;
	for(int ctr=0; ctr < int(font22_image.height) && (!pres); ++ctr) {
	  if(font22_image.pixel_data[4*(font22_image.width*ctr+xpos)+3])
	    { pres = 1; }
	  }
	++xpos;
	}

      flen[let] = xpos-foff[let];
      while((!pres) && xpos < (int)font22_image.width) {
	pres = 0;
	for(int ctr=0; ctr < int(font22_image.height) && (!pres); ++ctr) {
	  if(font22_image.pixel_data[4*(font22_image.width*ctr+xpos)+3])
	    pres = 1;
	  }
	++xpos;
	}
      --xpos;
      }
    }

  font = SDL_CreateRGBSurfaceFrom((void*)font22_image.pixel_data,
		font22_image.width, font22_image.height,
		32, 4*font22_image.width,
		0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
//  SDL_SetAlpha(font, SDL_RLEACCEL|SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
  SDL_SetAlpha(font, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
  }

int string_len(const char *str) {
  int ret=0;
  for(int ctr=0; ctr<int(strlen(str)); ++ctr) {
    ret += flen[str[ctr]];
    }
  return ret;
  }

void string_draw(SDL_Surface *s, int x, int y, unsigned long c, const char *t) {
  SDL_Rect sr, dr;
  int xpos = x;
  sr.y = 0;
  sr.h = 24;
  dr.y = y;

  for(int ctr=0; ctr<int(strlen(t)); ++ctr) {
    if(isgraph(t[ctr]) || t[ctr] == ' ') {
      sr.x = foff[t[ctr]];
      sr.w = flen[t[ctr]];
      dr.x = xpos;
      SDL_BlitSurface(font, &sr, s, &dr);
      xpos += flen[t[ctr]];
      }
    }
  }
