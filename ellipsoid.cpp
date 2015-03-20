#include "ellipsoid.h"
#include "vector.h"
#include "normal.h"
#include "ray.h"
#include <math.h>
#include <algorithm>
#include <iostream>

Ellipsoid::Ellipsoid(Point c, float rad, BRDF f) {
  this->center = c;
  this->f = f;
  this->m = Matrix(c, rad);
  this->inv = m.invert();
  this->radius = rad;
  this->transformed = 0;
}

Ellipsoid::Ellipsoid() {
  Ellipsoid(Point(), 0.0, BRDF());
}

bool Ellipsoid::intersection(Ray wr, LocalGeo* l) {
  // std::cout << wr.getDir().getX() << " " << wr.getDir().getY() << " " << wr.getDir().getZ() << "\n";
  

  Ray r;
  if (this->transformed) {
    r = worldToObj(wr);    
  } else {
    r = wr;
  }

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

    if (this->transformed) {
      p = objToWorld(p);
      
      Vector vn = Vector(n);
      vn = objToWorld(vn); 
      n = Normal(vn.getX(), vn.getY(), vn.getZ());
    }


    l->setPoint(p);
    l->setNormal(n);
  }

  return 1;
}

Normal Ellipsoid::getNormalAtPoint(Point p) {
  // Vector v = Vector((p.getX() - this->center.getX())/this->radius, (p.getY() - this->center.getY())/this->radius, (p.getZ() - this->center.getZ())/this->radius);
  Vector v = Vector(p, this->center);
  v = v * (1/this->radius);
  return Normal(v.getX(), v.getY(), v.getZ());
}

void Ellipsoid::transform(Transformation t) {
  this->transformed = 1;
  std::cout<< "in transform\n";
  this->m.print();
  this->m = this->m * t.getTrans();
  this->m.print();
  this->inv = this->inv * t.getInv();
  this->inv.print();
}

Ray Ellipsoid::worldToObj(Ray r) {
  Point p = r.getStart();
  Vector vP = Vector(p, Point());
  vP = this->inv * vP;
  p = Point(vP.getX(), vP.getY(), vP.getZ());

  Vector dir = r.getDir();
  // dir = this->inv * dir;

  // std::cout << dir.getX() << " " << dir.getY() << " " << dir.getZ() << "\n";

  dir = this->inv * dir;

  // this->m.print();
  // std::cout << "\n";

  // std::cout << dir.getX() << " " << dir.getY() << " " << dir.getZ() << "\n";



  return Ray(Point(vP.getX(), vP.getY(), vP.getZ()), this->inv * dir, r.getMin(), r.getMax());
}

Point Ellipsoid::objToWorld(Point p) {
  Vector temp = Vector(p, Point());
  temp = this->m * temp;

  return Point(temp.getX(), temp.getY(), temp.getZ());
}

Vector Ellipsoid::objToWorld(Vector v) {
  return this->m * v;
}
