#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "point.h"
#include "normal.h"
#include "ray.h"
#include "localgeo.h"
#include "BRDF.h"
#include "transformation.h"

class Triangle : public Shape {
  Point a, b, c;
  Normal n;
  Normal na;
  Normal nb;
  Normal nc;

public:
  Triangle(Point a, Point b, Point c, BRDF f);
  Triangle();
  Triangle(Point a, Point b, Point c, BRDF f, Normal na, Normal nb, Normal nc);

  /* If there is an intersection, return 1 and update l to the intersection point */
  bool intersection(Ray r, LocalGeo* l);
  Normal getNormal();


  /* Getter & Setter Methods */
  void setA(Point a);
  void setB(Point b);
  void setC(Point c);
  Point getA();
  Point getB();
  Point getC();

  void transform(Transformation t);   
  Ray worldToObj(Ray r);
  Point objToWorld(Point p);
  Vector objToWorld(Vector v);
  
};

#endif