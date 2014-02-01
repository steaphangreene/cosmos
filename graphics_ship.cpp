#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

#include "graphics_ship.h"
#include "renderer/linear.h"

#define SHIP_RADIUS     ((GLdouble)0.05)
#define POLYCOUNT       16

#define OBJECT_SPHERE     10
#define OBJECT_CUBE       11
#define OBJECT_PIPE       12
#define OBJECT_BEAM       13
#define OBJECT_DISK       14

static int ships_init = 0;
static map<char, int> objs;
static map<int, vector<float> > vecs;

static void set_vec(int shp, int vecnum, float cx, float cy, float cz, float dx, float dy, float dz, float lx, float ly, float lz) {
  vecs[shp*10+vecnum].push_back(cx);
  vecs[shp*10+vecnum].push_back(cy);
  vecs[shp*10+vecnum].push_back(cz);
  vecs[shp*10+vecnum].push_back(dx);
  vecs[shp*10+vecnum].push_back(dy);
  vecs[shp*10+vecnum].push_back(dz);
  vecs[shp*10+vecnum].push_back(lx);
  vecs[shp*10+vecnum].push_back(ly);
  vecs[shp*10+vecnum].push_back(lz);
  }

void create_sphere() {
  glNewList(OBJECT_SPHERE, GL_COMPILE);
  GLUquadricObj *quadObj;
  quadObj = gluNewQuadric();
  gluSphere(quadObj, SHIP_RADIUS, POLYCOUNT, POLYCOUNT/2);
  gluDeleteQuadric(quadObj);
  glEndList();

  set_vec(OBJECT_SPHERE, 0, 0.0, 0.0, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_SPHERE, 1, 0.0, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_SPHERE, 2, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_SPHERE, 3, 0.0, 0.0, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_SPHERE, 4, 0.0, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_SPHERE, 5, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  }

void create_pipe() {
  glNewList(OBJECT_PIPE, GL_COMPILE);
  glPushMatrix();
  GLUquadricObj *quadObj;
  quadObj = gluNewQuadric();
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glTranslatef(0.0, 0.0, -SHIP_RADIUS);
  gluQuadricOrientation(quadObj, GLU_INSIDE);
  gluCylinder(quadObj, SHIP_RADIUS*0.1, SHIP_RADIUS*0.1, SHIP_RADIUS*2.0, POLYCOUNT/2, 1);
  gluDeleteQuadric(quadObj);

  quadObj = gluNewQuadric();
  gluQuadricOrientation(quadObj, GLU_INSIDE);
  gluDisk(quadObj, 0.0, SHIP_RADIUS*0.1, POLYCOUNT/2, 1);
  gluDeleteQuadric(quadObj);

  glTranslatef(0.0, 0.0, SHIP_RADIUS*2.0);
  quadObj = gluNewQuadric();
  gluQuadricOrientation(quadObj, GLU_INSIDE);
  gluDisk(quadObj, 0.0, SHIP_RADIUS*0.1, POLYCOUNT/2, 1);
  gluDeleteQuadric(quadObj);

  glPopMatrix();
  glEndList();

  set_vec(OBJECT_PIPE, 0, 0.0, 0.0, SHIP_RADIUS*0.1, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_PIPE, 1, 0.0, SHIP_RADIUS*0.1, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_PIPE, 2, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_PIPE, 3, 0.0, 0.0, -SHIP_RADIUS*0.1, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_PIPE, 4, 0.0, -SHIP_RADIUS*0.1, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_PIPE, 5, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  }

void create_disk() {
  glNewList(OBJECT_DISK, GL_COMPILE);
  glPushMatrix();
  glScalef(1.0, 1.0, 0.20);
  glCallList(OBJECT_SPHERE);
  glPopMatrix();
  glEndList();

  set_vec(OBJECT_DISK, 0, 0.0, 0.0, SHIP_RADIUS*0.2, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_DISK, 1, 0.0, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_DISK, 2, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_DISK, 3, 0.0, 0.0, -SHIP_RADIUS*0.2, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_DISK, 4, 0.0, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_DISK, 5, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  }

void create_cube() {
  glNewList(OBJECT_CUBE, GL_COMPILE);
  glBegin(GL_QUADS);

  glNormal3d(0.0, -1.0, 0.0);
  glVertex3d(SHIP_RADIUS, -SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, -SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, -SHIP_RADIUS, SHIP_RADIUS);
  glVertex3d(SHIP_RADIUS, -SHIP_RADIUS, SHIP_RADIUS);

  glNormal3d(-1.0, 0.0, 0.0);
  glVertex3d(-SHIP_RADIUS, -SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, SHIP_RADIUS, SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, -SHIP_RADIUS, SHIP_RADIUS);

  glNormal3d(0.0, 1.0, 0.0);
  glVertex3d(-SHIP_RADIUS, SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(SHIP_RADIUS, SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(SHIP_RADIUS, SHIP_RADIUS, SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, SHIP_RADIUS, SHIP_RADIUS);

  glNormal3d(1.0, 0.0, 0.0);
  glVertex3d(SHIP_RADIUS, SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(SHIP_RADIUS, -SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(SHIP_RADIUS, -SHIP_RADIUS, SHIP_RADIUS);
  glVertex3d(SHIP_RADIUS, SHIP_RADIUS, SHIP_RADIUS);

  glNormal3d(0.0, 0.0, -1.0);
  glVertex3d(-SHIP_RADIUS,-SHIP_RADIUS, SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, SHIP_RADIUS, SHIP_RADIUS);
  glVertex3d( SHIP_RADIUS, SHIP_RADIUS, SHIP_RADIUS);
  glVertex3d( SHIP_RADIUS,-SHIP_RADIUS, SHIP_RADIUS);

  glNormal3d(0.0, 0.0, 1.0);
  glVertex3d(-SHIP_RADIUS,-SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d(-SHIP_RADIUS, SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d( SHIP_RADIUS, SHIP_RADIUS, -SHIP_RADIUS);
  glVertex3d( SHIP_RADIUS,-SHIP_RADIUS, -SHIP_RADIUS);

  glEnd();
  glEndList();

  set_vec(OBJECT_CUBE, 0, 0.0, 0.0, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_CUBE, 1, 0.0, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_CUBE, 2, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_CUBE, 3, 0.0, 0.0, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_CUBE, 4, 0.0, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_CUBE, 5, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  }

void create_beam() {
  glNewList(OBJECT_BEAM, GL_COMPILE);
  glPushMatrix();
  glScalef(1.0, 0.1, 0.1);
  glCallList(OBJECT_CUBE);
  glPopMatrix();
  glEndList();

  set_vec(OBJECT_BEAM, 0, 0.0, 0.0, SHIP_RADIUS*0.1, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_BEAM, 1, 0.0, SHIP_RADIUS*0.1, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_BEAM, 2, SHIP_RADIUS, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_BEAM, 3, 0.0, 0.0, -SHIP_RADIUS*0.1, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  set_vec(OBJECT_BEAM, 4, 0.0, -SHIP_RADIUS*0.1, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
  set_vec(OBJECT_BEAM, 5, -SHIP_RADIUS, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  }

static void init_ships() {
  ships_init = 1;
  objs['['] = OBJECT_CUBE;
  objs['o'] = OBJECT_SPHERE;
  objs['O'] = OBJECT_DISK;
  objs['|'] = OBJECT_PIPE;
  objs['I'] = OBJECT_BEAM;
  create_sphere();
  create_pipe();
  create_disk();
  create_cube();
  create_beam();
  }

static int last_obj = OBJECT_CUBE;

void handle_attach(int point) {
  int vecn = last_obj*10+point;
//  int vecn = last_obj*10+2; //POINT HARDCODED

  glTranslatef(vecs[vecn][0], vecs[vecn][1], vecs[vecn][2]);

  float matrix[16] = {
    vecs[vecn][3], vecs[vecn][4], vecs[vecn][5], 0.0,
    0.0, 0.0, 0.0, 0.0,
    vecs[vecn][6], vecs[vecn][7], vecs[vecn][8], 0.0,
    0.0, 0.0, 0.0, 1.0
    };
  cross(matrix[0], matrix[1], matrix[2], matrix[8], matrix[9], matrix[10], matrix[4], matrix[5], matrix[6]);
  norm(matrix[4], matrix[5], matrix[6]);

  glMultMatrixf(matrix);
  }

void handle_attached(int point) {
  int vecn = last_obj*10+point;
//  int vecn = last_obj*10+2; //POINT HARDCODED

  float matrix[16] = {
    -vecs[vecn][3], 0.0, vecs[vecn][6], 0.0,
    -vecs[vecn][4], 0.0, vecs[vecn][7], 0.0,
    -vecs[vecn][5], 0.0, vecs[vecn][8], 0.0,
    0.0, 0.0, 0.0, 1.0
    };
  cross(matrix[0], matrix[4], matrix[8], matrix[2], matrix[6], matrix[10], matrix[1], matrix[5], matrix[9]);
  norm(matrix[1], matrix[5], matrix[9]);

  glMultMatrixf(matrix);

  glTranslatef(-vecs[vecn][0], -vecs[vecn][1], -vecs[vecn][2]);
  }

static float flipy_matrix[16] = {
  1.0, 0.0, 0.0, 0.0,
  0.0, -1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0
  };


/* Debugging colors.
static float colors[][3] = {
  {0.4, 0.4, 0.4},
  {0.4, 0.4, 0.0},
  {0.0, 0.4, 0.0},
  {0.4, 0.0, 0.0},
  {0.0, 0.0, 0.4},
  {0.4, 0.0, 0.4},
  {0.0, 0.4, 0.4}
  };
*/

static float colors[][3] = {
  {0.4, 0.4, 0.4},
  {0.4, 0.4, 0.4},
  {0.4, 0.4, 0.4},
  {0.4, 0.4, 0.4},
  {0.4, 0.4, 0.4},
  {0.4, 0.4, 0.4},
  {0.4, 0.4, 0.4}
  };

void render_ship(const string &ship) {
  if(!ships_init) init_ships();

  int attach_point;
  int push_level;
  int color;
  string::const_iterator ch;

  color = 0;
  attach_point = -1;
  glPushMatrix();
  push_level = 1;
  ch = ship.begin();
  while(ch != ship.end()) {
    if(isdigit(*ch)) { attach_point = (*ch) - '0'; ++ch; }
    if(ch != ship.end() && objs.count(*ch)) {
      glColor3f(colors[color][0], colors[color][1], colors[color][2]); ++color;

      last_obj = objs[*ch];
      if(attach_point >= 0) handle_attached(attach_point);
      glCallList(last_obj);
      ++ch;

      if(ch != ship.end() && (*ch) == ' ') {
	glPopMatrix();
	--push_level;
	++ch;
	}

      glPushMatrix();
      ++push_level;

      if(ch != ship.end() && isdigit(*ch)) {
	handle_attach((*ch) - '0');
	++ch;
	}
      }
    else {
      fprintf(stderr, "Error: Invalid Ship Design: '%s' [%c]\n", ship.c_str(), *ch);
      ++ch;
      }
    }
  while(push_level > 0) {
    glPopMatrix();
    --push_level;
    }

  color = 0;
  attach_point = -1;
  glMultMatrixf(flipy_matrix);
  glPushMatrix();
  push_level = 1;
  ch = ship.begin();
  while(ch != ship.end()) {
    if(isdigit(*ch)) { attach_point = (*ch) - '0'; ++ch; }
    if(ch != ship.end() && objs.count(*ch)) {
      glColor3f(colors[color][0], colors[color][1], colors[color][2]); ++color;

      last_obj = objs[*ch];
      if(attach_point >= 0) handle_attached(attach_point);
      glCallList(last_obj);
      ++ch;

      //glColor3f(0.4, 0.4, 0.0);

      if(ch != ship.end() && (*ch) == ' ') {
	glPopMatrix();
	--push_level;
	++ch;
	}

      glPushMatrix();
      ++push_level;

      if(ch != ship.end() && isdigit(*ch)) {
	handle_attach((*ch) - '0');
	++ch;
	}
      }
    else {
      fprintf(stderr, "Error: Invalid Ship Design: '%s' [%c]\n", ship.c_str(), *ch);
      ++ch;
      }
    }
  while(push_level > 0) {
    glPopMatrix();
    --push_level;
    }
  }
