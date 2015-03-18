#include "ellipsoid.h"
#include "matrix.h"
#include "vector.h"
#include "normal.h"
#include "ray.h"
#include <math.h>
#include <algorithm>
#include <iostream>

Ellipsoid::Ellipsoid(Point c, float rad, BRDF f) {
  this->center = c;
  this->f = f;
  this->m = Matrix(c.x, c.y, c.z, 1); // TODO: INCORPORATE RADIUS ONTO DIAGONAL
  this->radius = rad;
}

Ellipsoid::Ellipsoid() {
  Ellipsoid(Point(), 0.0, BRDF());
}

/* RIGHT NOW ONLY WORKS FOR SPHERES, NOT ELLIPSOIDS 
   TODO: make work for arbitrary ellipsoid, might need to do stuff with world space / object space*/
bool Ellipsoid::intersection(Ray r, LocalGeo* l) {
  Vector dir = r.getDir();
  dir.normalize();
  Point start = r.getStart();
  Point cen = this->center;
  float rad = this->radius;

  float a = dir.dot(dir);
  float b = dir.dot(Vector(start, cen)) * 2;
  float c = (Vector(start, cen)).dot(Vector(start, cen)) - pow(rad, 2);

  float det = pow(b, 2) - 4*a*c;
  float t;
  if (det < 0) { // NO INTERSECTION
    return 0;
  } else if (det == 0) { // ONE SOLUTION
    t = (-b)/2*a;
    if (!r.inRange(t)) {
      return 0;
    }
  } else { // TWO SOLUTIONS 
    float t1 = (-b + sqrt(det))/2*a;
    float t2 = (-b - sqrt(det))/2*a;
    if (t1 * t2 < 0) { // ray starts inside of the sphere
      t = std::max(t1, t2);
    } else if (!(r.inRange(t1)) && !(r.inRange(t2))) { // both t out of range
      return 0;
    } else if (!r.inRange(t1)) {
      t = t2;
    } else if (!r.inRange(t2)) {
      t = t1;
    } else { // return closer t value
      t = std::min(t1, t2);      
    }
  }

  Point p = r.getPointAtT(t);
  Normal n = this->getNormalAtPoint(p);

  l->setPoint(p);
  l->setNormal(n);

  return 1;
}

/* Only works for spheres. 
  TODO: implement for ellipsoid */
Normal Ellipsoid::getNormalAtPoint(Point p) {
  return Normal((p.x - this->center.x)/this->radius, (p.y - this->center.y)/this->radius, (p.z - this->center.z)/this->radius);
}

