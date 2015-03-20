#include "ellipsoid.h"
#include "vector.h"
#include "normal.h"
#include "ray.h"
#include <math.h>
#include <algorithm>
#include <iostream>

Ellipsoid::Ellipsoid(Point c, float rad, BRDF f) {
  this->center = Point();
  this->f = f;
  this->m = Matrix(c, rad);
  this->inv = m.invert();
  this->radius = 1.0;
}

Ellipsoid::Ellipsoid() {
  Ellipsoid(Point(), 0.0, BRDF());
}

bool Ellipsoid::intersection(Ray wr, LocalGeo* l) {
  // std::cout << wr.getDir().getX() << " " << wr.getDir().getY() << " " << wr.getDir().getZ() << "\n";
  
  // std::cout << "IN INTERSECTION \n";

  Ray r;
  r = worldToObj(wr);    


  Point start = r.getStart();
  Vector dir = r.getDir();
  dir.normalize();

  Point cen = this->center;
  float rad = this->radius;

  float a = dir.dot(dir);
  float b = dir.dot(Vector(start, cen)) * 2;
  float c = (Vector(start, cen)).dot(Vector(start, cen)) - pow(rad, 2);

  // std::cout << a << "\n";
  // std::cout << dir.getX() << " " << dir.getY() << " " << dir.getZ() << "\n";

  float det = pow(b, 2) - 4*a*c;
  float t;
  if (det < 0) { // NO INTERSECTION
    return 0;
  } else { // TWO SOLUTIONS 
    // std::cout << "positive det \n";

    float t1 = (-b + sqrt(det))/2*a;
    float t2 = (-b - sqrt(det))/2*a;
    if (t1 * t2 < 0) { // ray starts inside of the sphere

      return 0;
    } else if (det == 0) { // ONE SOLUTION
      if (r.inRange(t1)) {
        t = t1;
      }
    } else if (!(r.inRange(t1)) && !(r.inRange(t2))) { // both t out of range
      // std::cout << t1 << " " << t2 << "\n";
      return 0;
    } else if (!r.inRange(t1)) {
      t = t2;
    } else if (!r.inRange(t2)) {
      t = t1;
    } else { // return closer t value
      t = std::min(t1, t2);      
    }

    Vector temp = (dir * t) + start;
    Point p = Point(temp.getX(), temp.getY(), temp.getZ());

    Normal n = this->getNormalAtPoint(p);


      p = objToWorld(p);
      
      Vector vn = Vector(n);
      vn = objToWorld(vn); 
      n = Normal(vn.getX(), vn.getY(), vn.getZ());


    l->setPoint(p);
    l->setNormal(n);
  }

  return 1;
}

Normal Ellipsoid::getNormalAtPoint(Point p) {
  // Vector v = Vector((p.getX() - this->center.getX())/this->radius, (p.getY() - this->center.getY())/this->radius, (p.getZ() - this->center.getZ())/this->radius);
  Vector v = Vector(p, this->center);
  // v = v * (1/this->radius);
  return Normal(v.getX(), v.getY(), v.getZ());
}

void Ellipsoid::transform(Transformation t) {
  std::cout<< "in transform\n";
  this->m.print();
  this->m = t.getTrans() * this->m;
  t.getTrans().print();
  this->m.print();
  this->inv = this->inv *  t.getInv();
  this->inv.print();
}

Ray Ellipsoid::worldToObj(Ray r) {
  Point p = this->inv * r.getStart();

  Vector dir = this->inv * r.getDir();
  
  return Ray(p, dir, r.getMin(), r.getMax());
}

Point Ellipsoid::objToWorld(Point p) {
  return this->m * p;
}

Vector Ellipsoid::objToWorld(Vector v) {
  return this->m * v;
}

