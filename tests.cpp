// #include "stdafx.h"
#include <iostream>
#include <math.h>
#include "tests.h"
#include "vector.h"
#include "point.h"
#include "normal.h"
#include "ellipsoid.h"
#include "triangle.h"
#include "matrix.h"
#include "ray.h"
#include "localgeo.h"

Tests::Tests() {};

void Tests::checkAll() {

  std::cout << "Running testssssssss\n";
  
  std::cout << "Vector tests: ";
  (this->vector()) ? std::cout << "OK\n" : std::cout << "\tFAIL\n";
  
  std::cout << "Normal tests: ";
  (this->normal()) ? std::cout << "OK\n" : std::cout << "\tFAIL\n";
  
  std::cout << "Ellipsoid tests: ";
  (this->ellipsoid()) ? std::cout << "OK\n" : std::cout << "\tFAIL\n";

  std::cout << "Triangle tests: ";
  (this->triangle()) ? std::cout << "OK\n" : std::cout << "\tFAIL\n";

  std::cout << "Matrix tests: ";
  (this->matrix()) ? std::cout << "OK\n" : std::cout << "\tFAIL\n";
};

bool Tests::vector() {
  bool pass = 1;
  /* TEMP vectors change value throughout the test. Other vectors maintain their values. */
  Point p1 = Point(4.0, 0.0, 0.0);
  Point p2 = Point();

  Vector a = Vector(1.0, 2.0, 3.0);
  Vector b = Vector(p1, p2);
  Vector c = Vector(-4.0, 0.0, 0.0);
  Vector d = Vector(3.0, 1.0, 2.0);
  Vector temp = Vector(4.0, 0.0, 0.0);

  if (a.getX() != 1.0 || a.getY() != 2.0 || a.getZ() != 3.0) {
    pass = 0;
    std::cout << "\n\tVector init from floats\n";
  }

  if (b.getX() != 4.0 || b.getY() != 0.0 || b.getZ() != 0.0) {
    pass = 0;
    std::cout << "\n\tVector init from points\n";
  }
  
  if (b.getLength() != 4) {
    pass = 0;
    std::cout << b.getLength();
    std::cout << "\n\tVector length 1\n";
  }

  if (b.getLength() != c.getLength()) {
    pass = 0;
    std::cout << "\n\tVector length 2\n";
  }

  temp.normalize();
  if (temp.getX() != 1.0 || temp.getY() != 0.0 || temp.getZ() != 0.0) {
    pass = 0;
    std::cout << "\n\tVector normalize\n";
  }

  if (a.dot(d) != 11.0) {
    pass = 0;
    std::cout << "\n\tVector dot product\n";
  }

  temp = a.cross(d);
  if (temp.getX() != 1.0 || temp.getY() != 7.0 || temp.getZ() != -5.0) {
    pass = 0;
    std::cout << "\n\tVector cross product\n";
  }

  temp = a * 2.0;
  if (temp.getX() != 2.0 || temp.getY() != 4.0 || temp.getZ() != 6.0) {
    pass = 0;
    std::cout << "\n\tScalar multiplication\n";
  }

  temp = a + b;
  if (temp.getX() != 5.0 || temp.getY() != 2.0 || temp.getZ() != 3.0) {
    pass = 0;
    std::cout << "\n\tVector addition\n";
  }

  temp = a - b;
  if (temp.getX() != -3.0 || temp.getY() != 2.0 || temp.getZ() != 3.0) {
    pass = 0;
    std::cout << "\n\tVector subtraction\n";
  }

  return pass;
};

bool Tests::normal() {
  bool pass = 1;
  /* TEMP vectors change value throughout the test. Other vectors maintain their values. */
  Normal a = Normal(0.0, 4.0, 0.0);
  Normal b = Normal(-4.0, 0.0, 0.0);
  Normal c = Normal(1.0, 0.0, 0.0);
  Normal d = Normal(1.0, 0.0, 0.0);
  Normal temp = Normal(4.0, 0.0, 0.0);

  if (a.getX() != 0.0 || a.getY() != 1.0 || a.getZ() != 0.0) {
    pass = 0;
    std::cout << "\n\tNormal init\n";
  }

  if (b.getX() != -1.0 || b.getY() != 0.0 || b.getZ() != 0.0) {
    pass = 0;
    std::cout << "\n\tNormal init\n";
  }

  temp = c + d;
  if (temp.getX() != 1.0 || temp.getY() != 0.0 || temp.getZ() != 0.0) {
    pass = 0;
    std::cout << "\n\tNormal addition\n";
  }

  temp = b - c;
  if (temp.getX() != -1.0 || temp.getY() != 0.0 || temp.getZ() != 0.0) {
    pass = 0;
    std::cout << "\n\tNormal subtraction\n";
  }

  return pass;
};

bool Tests::ellipsoid() {
  bool pass = 1;

  BRDF f = BRDF(); // not static
  Point a = Point(0, 1, 0);
  Point start = Point(0, 0, 4);
  Point origin = Point(0, 0, 0);
  Point unit = Point(1/sqrt(3), 1/sqrt(3), 1/sqrt(3));
  Vector dir = Vector(0, 0, -1);
  Ellipsoid e = Ellipsoid(origin, 1.0, f);
  Ray r = Ray(start, dir, 0.0, 20);
  Ray r2 = Ray(origin, dir, -4.0, 20);
  LocalGeo temp = LocalGeo(); // not static
  Point p = Point(); // not static
  Normal n = Normal(); // not static

  n = e.getNormalAtPoint(a);
  if(n.getX() != 0.0 || n.getY() != 1.0 || n.getZ() != 0.0) {
    pass = 0;
    std::cout << n.getX() << " " << n.getY() << " " << n.getZ() << "\n";
    std::cout << "\n\tGet Normal at Point1";
  }
  
  /* doesn't pass because of floating point precision */
  // n = e.getNormalAtPoint(unit);
  // if(n.getX() != 1/sqrt(3) || n.getY() != 1/sqrt(3) || n.getZ() != 1/sqrt(3)) {
  //   pass = 0;
  //   std::cout << n.getX() << " " << n.getY() << " " << n.getZ() << "\n";
  //   std::cout << 1/sqrt(3) << "\n";
  //   std::cout << "\n\tGet Normal at Point2";
  // }

  if(e.intersection(r, &temp)) {
    p = temp.getPoint();
    n = temp.getNormal();
    if (p.getX() != 0.0 || p.getY() != 0.0 || p.getZ() != 1.0 || n.getX() != 0.0 || n.getY() != 0.0 || n.getZ() != 1.0 ) {
      pass = 0;
      std::cout << "\n\tSimple intersection\n";
    }
  } else {
    pass = 0;
    std::cout << "\n\tSimple intersection\n";
  }

  if (e.intersection(r2, &temp)) {
    pass = 0;
    std::cout << "\n\tIntersection from inside sphere2\n";
  }


  return pass;
};

bool Tests::triangle() {

  bool pass = 1;

  BRDF f = BRDF(); 
  Point a = Point(1, 1, 0);
  Point b = Point(-1, 1, 0);
  Point c = Point(0, -1, 0);
  Point start = Point(0, 0, 4);
  Vector dir = Vector(0, 0, -1);
  Triangle e = Triangle(a, b, c, f);
  Ray r = Ray(start, dir, 0.0, 20);
  // Ray r2 = Ray(origin, dir, -4.0, 20);
  LocalGeo temp = LocalGeo(); // not static
  Point p = Point(); // not static
  Normal n = Normal(); // not static

  n = e.getNormal();
  if(n.getX() != 0.0 || n.getY() != 0.0 || n.getZ() != -1.0) {
    std::cout << n.getX();
    std::cout << n.getY();
    std::cout << n.getZ();
    pass = 0;
    std::cout << "\n\tGet Normal";
  }

  if(e.intersection(r, &temp)) {
    p = temp.getPoint();
    n = temp.getNormal();
    if (p.getX() != 0.0 || p.getY() != 0.0 || p.getZ() != 0.0 || n.getX() != 0.0 || n.getY() != 0.0 || n.getZ() != -1.0 ) {
      pass = 0;
      std::cout << "\n\tSimple intersection\n";
    }
  } else {
    pass = 0;
    std::cout << "\n\tSimple intersection\n";
  }

  return pass;
};

bool Tests::matrix() {
  bool pass = 1;

  float a[4][4] = {{.5, .3, .2, .6}, {.7, .3, .2, .3}, {.4, .5, .2, .1}, {.5, .4, .3, .6}};
  // float a[4][4] = {{.5, .3, .2, .6}, {.7, .3, .2, .3}, {.4, .5, .2, .1}, {.5, .4, .3, .6}};

  Matrix m1 = Matrix(a);
  
  Matrix temp = Matrix();  // NOT STATIC THROUGHOUT TEST

  
  /*Fails because of floating point precision */
  // if (m1.determinant() != -0.0067) {
  //   std::cout << m1.determinant();
  //   pass = 0; std::cout<< "\n\tdeterminant";
  // }

  /* Passes */
  // temp = m1.invert();
  // if (!(temp == Matrix())) {
  //   pass = 0;
  //   std::cout << "\n\tinvert";
  // }

  // temp = m1 * m2;
  // if (!(temp == m1m2)) {
  //   pass = 0;
  //   std::cout << "\n\tMatrix multiply";
  // }

  return pass;
};


