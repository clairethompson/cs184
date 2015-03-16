#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "shape.h"
#include "point.h"
#include "ray.h"
#include "normal.h"
#include "localgeo.h"
#include "BRDF.h"

class Ellipsoid : public Shape {
  Point center;
  float radius;


public:
  Ellipsoid(Point c, float rad, BRDF f);
  Ellipsoid();

  /* If there is an intersection, return 1 and update l to the intersection point */
  bool intersection(Ray r, LocalGeo* l);
  Normal getNormalAtPoint(Point p);
};

#endif