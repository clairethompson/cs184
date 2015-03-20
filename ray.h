#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"

class Ray {
  Point start;
  Vector dir;
  float t_min, t_max;

public:
  Ray(Point p, Vector v, float min, float max);
  Ray();
  Point getStart();
  Vector getDir();
  bool inRange(float t);
  float getMin();
  float getMax();
  Point getPointAtT(float t);
};

#endif