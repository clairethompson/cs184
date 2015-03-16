#include "point.h"
Point::Point (float a, float b, float c) : x(a), y(b), z(c) {};

Point::Point () {
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

float Point::getX() { return this->x; }
float Point::getY() { return this->y; }
float Point::getZ() { return this->z; }

