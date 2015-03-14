#include "ray.h"

Ray::Ray(Point p, Vector v, float min, float max) {
  this->start = p;
  this->dir = v;
  this->t_min = min; 
  this->t_max = max;
}

Ray::Ray() {
  Ray(Point(), Vector(), 0.0, 0.0);
}

Point Ray::getStart() {
  return this->start;
}

Vector Ray::getDir() {
  return this->dir;
}

bool Ray::inRange(float t) {
  if (this->t_min < t < this->t_max) {
    return 1;
  }
  return 0;
}