#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "point.h"
#include "normal.h"
#include "ray.h"
#include "localgeo.h"
#include "BRDF.h"

class Triangle : public Shape {
  Point a, b, c;
  Normal n;


public:
  Triangle(Point a, Point b, Point c, BRDF f);
  Triangle();

  /* If there is an intersection, return 1 and update l to the intersection point */
  bool intersection(Ray r, LocalGeo* l);
  Normal getNormal();
};

#endif