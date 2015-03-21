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

  this->na = Normal();
  this->nb = Normal();
  this->nc = Normal();

  this->alpha = 0.0;
  this->beta = 0.0;
  this->gamma = 0.0;

}

Triangle::Triangle() {
  Triangle(Point(), Point(), Point(), BRDF(), Normal(), Normal(), Normal());
}

Triangle::Triangle(Point a, Point b, Point c, BRDF f, Normal na, Normal nb, Normal nc) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->f = f;
  this->na = na;
  this->nb = nb;
  this->nc = nc;

  Vector ab = Vector(a, b);
  Vector ac = Vector(a, c);
  Vector temp = ab.cross(ac);
  this->n = Normal(temp.x, temp.y, temp.z);

  this->m = Matrix(); 
  this->inv = Matrix();

  Point centroid = Point((v1.getX() + v2.getX() + v3.getX())/3.0, (v1.getY() + v2.getY() + v3.getY())/3.0,
    (v1.getX() + v3.getY() + v3.getZ())/3.0);
  Vector cent_v1 = Vector(centroid, v1);
  Vector cent_v2 = Vector(centroid, v2);
  Vector cent_v3 = Vector(centroid, v3);
  float len_cent_v1 = cent_v1.getLength();
  float len_cent_v2 = cent_v2.getLength();
  float len_cent_v3 = cent_v3.getLength();
  float s_A1 = (len_cent_v2 + len_cent_v3 + e3.getLength()) / 2.0;
  float s_A2 = (len_cent_v1 + len_cent_v3 + e2.getLength()) / 2.0;
  float s_A3 = (len_cent_v1 + len_cent_v2 + e1.getLength()) / 2.0;
  float A1 = sqrt(s_A1 * (S_A1 - len_cent_v2) * (S_A1 - len_cent_v3) * (S_A1 - e3.getLength()));
  float A2 = sqrt(s_A2 * (S_A2 - len_cent_v1) * (S_A2 - len_cent_v3) * (S_A2 - e2.getLength()));
  float A3 = sqrt(s_A3 * (S_A3 - len_cent_v1) * (S_A3 - len_cent_v2) * (S_A3 - e1.getLength()));

  this->alpha = A1 / (A1 + A2 + A3);
  this->beta = A2 / (A1 + A2 + A3);
  this->gamma = A3 / (A1 + A2 + A3);
}

/* Moller-Trumbore Algorithm
see: http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm */
bool Triangle::intersection(Ray r, LocalGeo* l) {

  Point v1 = this->a;  // Triangle vertices
  Point v2 = this->b;
  Point v3 = this->c;
  Point O = r.getStart(); // Ray origin
  Vector D = r.getDir();  //Ray direction
 
  //Find vectors for two edges sharing V1
  Vector e1 = Vector(v2, v1);
  Vector e2 = Vector(v3, v1);

  Vector e3 = Vector(v2, v3);

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
    Point p = r.getPointAtT(t);

    Normal n;
    if (this->alpha != 0.0){
      n = Normal(alpha * this->na.getX() + alpha * this->nb.getX() + alpha * this->nc.getX(),
                      beta * this->na.getY() + beta * this->nb.getY() + beta * this.nc.getY(),
                      gamma * this->na.getZ() + gamma * this->nb.getZ() + gamma * this->nc.getZ());
    } else {
      n = this->n;
    }

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

/* Getter & Setter methods */
void Triangle::setA(Point a) { this->a = a; }
void Triangle::setB(Point b) { this->b = b; }
void Triangle::setC(Point c) { this->c = c; }
Point Triangle::getA() { return this->a; }
Point Triangle::getB() { return this->b; }
Point Triangle::getC() { return this->c; }

void Triangle::transform(Transformation t) {
  this->m = t.getTrans() * this->m;

  this->a = this->m * a;
  this->b = this->m * b;
  this->c = this->m * c;
  this->n = getNormal();

  this->inv = this->inv *  t.getInv();
}

