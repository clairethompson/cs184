#include "ellipsoid.h"
#include "triangle.h"
#include "matrix.h"
#include "vector.h"
#include "normal.h"
#include "ray.h"
#include <math.h>
#include <algorithm>
#include <iostream>

Triangle::Triangle(Point a, Point b, Point c, BRDF f) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->f = f;

  Vector ab = Vector(a, b);
  Vector ac = Vector(a, c);
  Vector temp = ab.cross(ac);
  this->n = Normal(temp.x, temp.y, temp.z);

  this->m = Matrix(); 
  this->inv = Matrix();
}

Triangle::Triangle() {
  Triangle(Point(), Point(), Point(), BRDF());
}

/* Moller-Trumbore Algorithm
see: http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm */
bool Triangle::intersection(Ray wr, LocalGeo* l) {
  Ray r = worldToObj(wr);

  Point v1 = this->a;  // Triangle vertices
  Point v2 = this->b;
  Point v3 = this->c;
  Point O = r.getStart(); // Ray origin
  Vector D = r.getDir();  //Ray direction
 
  //Find vectors for two edges sharing V1
  Vector e1 = Vector(v2, v1);
  Vector e2 = Vector(v3, v1);

  //Begin calculating determinant - also used to calculate u parameter
  Vector P = D.cross(e2);
  //if determinant is zero, ray lies in plane of triangle
  float det = e1.dot(P);;
  //NOT CULLING
  if(det == 0) {return 0;}
  float inv_det = 1 / det;
 
  //calculate distance from V1 to ray origin
  Vector T = Vector(O, v1);
 
  //Calculate u parameter and test bound
  float u = T.dot(P) * inv_det;
  //The intersection lies outside of the triangle
  if(u < 0.0f || u > 1.0f) {return 0;}
 
  //Prepare to test v parameter
  Vector Q = T.cross(e1);
 
  //Calculate V parameter and test bound
  float v = D.dot(Q) * inv_det;
  //The intersection lies outside of the triangle
  if(v < 0.0f || u + v  > 1.0f) {return 0;}
 
  float t = e2.dot(Q) * inv_det;
 
  if(t > 0) { //ray intersection
    Point p = objToWorld(r.getPointAtT(t));
    Normal n = this->getNormal();

    Vector temp = Vector(n.getX(), n.getY(), n.getZ());
    temp = objToWorld(temp);

    n = Normal(temp.getX(), temp.getY(), temp.getZ());

    l->setPoint(p);
    l->setNormal(n);

    return 1;
  }
 
  // No hit, no win
  return 0;
}

Normal Triangle::getNormal() {
  Vector e1 = Vector(this->b, this->a);
  Vector e2 = Vector(this->c, this->a);
  Vector temp = e1.cross(e2);
  return Normal(temp.x, temp.y, temp.z);
}


void Triangle::transform(Transformation t) {
  std::cout<< "in transform\n";
  this->m.print();
  this->m = t.getTrans() * this->m;
  t.getTrans().print();
  this->m.print();
  this->inv = this->inv *  t.getInv();
  this->inv.print();
}

Ray Triangle::worldToObj(Ray r) {
  Point p = this->inv * r.getStart();

  Vector dir = this->inv * r.getDir();

  return Ray(p, dir, r.getMin(), r.getMax());
}

Point Triangle::objToWorld(Point p) {
  // Vector temp = Vector(p, Point());
  // temp = this->m * temp;

  return this->m * p;
}

Vector Triangle::objToWorld(Vector v) {
  return this->m * v;
}

