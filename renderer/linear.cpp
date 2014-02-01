#include <cmath>

using namespace std;

#include "linear.h"

float span(float x1, float y1, float z1, float x2, float y2, float z2) {
  return acos( dot(x1, y1, z1, x2, y2, z2) / (mag(x1, y1, z1) * mag(x2, y2, z2)) );
  }

float dot(float x1, float y1, float z1, float x2, float y2, float z2) {
  return x1*x2+y1*y2+z1*z2;
  }

void cross(float x1, float y1, float z1, float x2, float y2, float z2, float &xa, float &ya, float &za) {
  xa = y1 * z2 - y2 * z1;
  ya = z1 * x2 - z2 * x1;
  za = x1 * y2 - x2 * y1;
  }

float mag(float x, float y, float z) {
  return sqrt(x*x+y*y+z*z);
  }

void norm(float x, float y, float z, float &xa, float &ya, float &za) {
  float mg = mag(x, y, z);
  xa = x/mg;
  ya = y/mg;
  za = z/mg;
  }

void norm(float &x, float &y, float &z) {
  float mg = mag(x, y, z);
  x = x/mg;
  y = y/mg;
  z = z/mg;
  }
