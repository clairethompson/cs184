#include "point.h"
Point::Point (float a, float b, float c) : x(a), y(b), z(c) {};

Point::Point () {
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

<<<<<<< HEAD
float Point::getX() { return this->x; }
float Point::getY() { return this->y; }
float Point::getZ() { return this->z; }

=======
float Point::getX() {
  return this->x;
}
float Point::getY() {
  return this->y;
}
float Point::getZ() {
  return this->z;
}
>>>>>>> 23611fcb28cc6b8173476c5e79c331fa41a79a3f
