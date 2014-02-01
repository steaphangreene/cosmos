// *************************************************************************
// renderer.c
// This is a component of Acid Rain, Pre ALPHA non-distribution version
//
// -By Insomnia (Steaphan Greene)   (Copyright 2002 Steaphan Greene)
//                   (stea@core.binghamton.edu)
//      No waranty stated or implied, I am not responsible for any damage
// caused directly or indirectly by this software.
//      Permision granted for use/distribution/modification by anyone,
// provided this header remains intact, and modified versions are marked
// as so immediately below this header.
//      Products utilizing the code or methods within may be distributed
// freely along with this licence, but any sales for profit of such products
// must have the author's permission, and may be subject to a royaltee fee.
// *************************************************************************

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "settings.h"
#include "renderer.h"

static SDL_Surface *surface = NULL;
static int videoFlags = 0;

static int xsize=0, ysize=0;
static int hgap=0, vgap=0;

int fullscreen_mode = 0;

//int settings_window_open = 0;

viewport cview = {0.0, 0.0, 0.0, 0.0, 0, 0, 0, 1.0};

int phase = 0;

void load_textures(void) {
  }

int init_renderer(int xs, int ys) {
  const SDL_VideoInfo *videoInfo;
  GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat shininess[] = { 100.0 };
  GLfloat light_pos[] = { 1.0, -1.0, -1.0, 0.0 };

  xsize = xs;   ysize = ys;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    return 0;
    }
  atexit(SDL_Quit);

  videoInfo = SDL_GetVideoInfo();

  videoFlags = SDL_OPENGL;
  videoFlags |= SDL_GL_DOUBLEBUFFER;
  videoFlags |= SDL_HWPALETTE;
  videoFlags |= SDL_RESIZABLE;
  if(fullscreen_mode)
    videoFlags |= SDL_FULLSCREEN;

  /* Use HW Survaces if possible */
  if(videoInfo->hw_available) videoFlags |= SDL_HWSURFACE;
  else videoFlags |= SDL_SWSURFACE;

  /* Use HW Blits if possible */
  if(videoInfo->blit_hw) videoFlags|=SDL_HWACCEL;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  surface = SDL_SetVideoMode(xsize, ysize, 0, videoFlags);

  if(surface == NULL) {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    return 0;
    }

  /* Set a window title. */
  SDL_WM_SetCaption("Cosmos: Ships Demo", "Cosmos: Ships Demo");

  // Set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // Set the shading model
  glShadeModel(GL_SMOOTH);
//  glShadeModel(GL_FLAT);

  // Set the polygon mode to fill
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Fuck with Alpha shit
  glDepthFunc(GL_LEQUAL);

  // Enable depth testing for hidden line removal
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

//  glCullFace (GL_BACK);
//  glEnable (GL_CULL_FACE);

//  glEnable (GL_POLYGON_SMOOTH);
//  if(glError());
//  glHint(GL_POLYGON_SMOOTH,GL_FASTEST);

//  glEnable(GL_POINT_SMOOTH);
//  glEnable(GL_LINE_SMOOTH);



//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

  // Define material properties of specular color and degree of 
  // shininess.  Since this is only done once in this particular 
  // example, it applies to all objects.  Material properties can 
  // be set for individual objects, individual faces of the objects,
  // individual vertices of the faces, etc... 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  // Set the GL_AMBIENT_AND_DIFFUSE color state variable to be the
  // one referred to by all following calls to glColor
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  // Create a Directional Light Source
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Set the new viewport size
  glViewport(0, 0, (GLint)xsize, (GLint)ysize);

  load_textures();

  return 1;
  }

int start_scene(scene *cscene, int player) {
  static Uint32 lasttick = 0;
  Uint32 difftick = SDL_GetTicks()-lasttick;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  ++phase;

  if(lasttick == 0) {
    lasttick = SDL_GetTicks();
    difftick = 30;
    }
  else lasttick += 30;

  if(difftick < 0 || difftick >= 1000000) {
    int d = (int)difftick;
    SDL_Delay(-d);
    }

  if(difftick > 60 && difftick < 1000000) {
    return -1;
    }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.8, 0.8, 0.6, -0.6, 5.0, 20.0);
  glMatrixMode(GL_MODELVIEW);

  return 1;
  }

int finish_scene(scene *cscene, int player) {
  glFlush();
  SDL_GL_SwapBuffers();

  return 1;
  }

void resize_display(int xs, int ys) {
  int rx = xs, ry = ys;
  surface = SDL_SetVideoMode(rx, ry, 16, videoFlags);

  xsize = xs;   ysize = ys;

  if(xsize > (ysize*4)/3) xsize = (ysize*4)/3;
  if(ysize > (xsize*3)/4) ysize = (xsize*3)/4;

  hgap = (rx-xsize)/2;  vgap = (ry-ysize)/2;
  glViewport(hgap, vgap, (GLint)xsize, (GLint)ysize);
  }

static int oldmodex = 0, oldmodey = 0;

//void toggle_settings_window(void) {
//  settings_window_open = !settings_window_open;
//  }

void toggle_fullscreen(void) {
  char drv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  SDL_VideoDriverName((char *)drv, 15);
  if(strcmp(drv, "x11")) return;

  if(oldmodex != 0) {
    resize_display(oldmodex, oldmodey);
    oldmodex = 0;  oldmodey = 0;
    }
  else {
    int ctr, goal=-1;
    SDL_Rect **modes;

    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

    if(modes == (SDL_Rect **)0){
      modes = SDL_ListModes(NULL, SDL_FULLSCREEN);
      }
    if(modes == (SDL_Rect **)0){
      return;
      }

    oldmodex = surface->w;
    oldmodey = surface->h;

    for(ctr=0; modes[ctr]; ++ctr) {
      if(modes[ctr]->w >= surface->w && modes[ctr]->h >= surface->h) {
	if(goal == -1
		|| modes[ctr]->w < modes[goal]->w
		|| modes[ctr]->h < modes[goal]->h) {
	  goal = ctr;
	  }
	}
      }
    if(goal == -1) {
      goal = 0;
      for(ctr=1; modes[ctr]; ++ctr) {
	if(modes[ctr]->w > modes[goal]->w || modes[ctr]->h > modes[goal]->h) {
	  goal = ctr;
	  }
	}
      }
    resize_display(modes[goal]->w, modes[goal]->h);
    }
  SDL_WM_ToggleFullScreen(surface);
  videoFlags ^= SDL_FULLSCREEN;
  fullscreen_mode = (videoFlags & SDL_FULLSCREEN) != 0;
  }

void pixels_to_location(double *x, double *y) {
  (*x) -= (double)hgap;
  (*y) -= (double)vgap;

  if((*x) < 0.0) (*x)=-10.0, (*y)=-10.0;
  else if((*y) < 0.0) (*x)=-10.0, (*y)=-10.0;
  else if((*x) >= (double)xsize) (*x)=-10.0, (*y)=-10.0;
  else if((*y) >= (double)ysize) (*x)=-10.0, (*y)=-10.0;
  else {
    (*x) /= (double)ysize;  // INTENTIONAL - Divide by YSIZE, not XSIZE!
    (*y) /= (double)ysize;
    (*x) *= 2.0;  (*x) -= 1.0;
    (*y) *= 2.0;  (*y) -= 1.0;
    }
  }

void load_xpm_texture(unsigned int tex, char *xpm[]) {
  int width, height, ncol, x, y;
  unsigned char *tmp;

  sscanf(xpm[0], "%d %d %d", &width, &height, &ncol);
  tmp = (unsigned char *)malloc(width*height*4);
//  memset(tmp, 0, width*height*4);
  for(y=0; y<height; ++y) {
    for(x=0; x<width; ++x) {
      if(xpm[1+ncol+y][x] == xpm[1][0]) {
	tmp[(y*width+x)*4+0] = 0;
	tmp[(y*width+x)*4+1] = 0;
	tmp[(y*width+x)*4+2] = 0;
	tmp[(y*width+x)*4+3] = 0;
	}
      else {
	tmp[(y*width+x)*4+0] = 255;
	tmp[(y*width+x)*4+1] = 255;
	tmp[(y*width+x)*4+2] = 255;
	tmp[(y*width+x)*4+3] = 255;
	}
      }
    }

  glBindTexture(GL_TEXTURE_2D, tex);

/////////
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                 GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the original
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // the texture wraps over at the edges (repeat)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
/////////

  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height,
	GL_RGBA, GL_UNSIGNED_BYTE, tmp);
//  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height,
//	GL_COLOR_INDEX, GL_BITMAP, tmp);
//  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  free(tmp);
  }
