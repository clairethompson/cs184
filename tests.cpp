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

  std::cout << "Running tests\n";
  
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

  if (a.x != 1.0 || a.y != 2.0 || a.z != 3.0) {
    pass = 0;
    std::cout << "\n\tVector init from floats\n";
  }

  if (b.x != 4.0 || b.y != 0.0 || b.z != 0.0) {
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
  if (temp.x != 1.0 || temp.y != 0.0 || temp.z != 0.0) {
    pass = 0;
    std::cout << "\n\tVector normalize\n";
  }

  if (a.dot(d) != 11.0) {
    pass = 0;
    std::cout << "\n\tVector dot product\n";
  }

  temp = a.cross(d);
  if (temp.x != 1.0 || temp.y != 7.0 || temp.z != -5.0) {
    pass = 0;
    std::cout << "\n\tVector cross product\n";
  }

  temp = a * 2.0;
  if (temp.x != 2.0 || temp.y != 4.0 || temp.z != 6.0) {
    pass = 0;
    std::cout << "\n\tScalar multiplication\n";
  }

  temp = a + b;
  if (temp.x != 5.0 || temp.y != 2.0 || temp.z != 3.0) {
    pass = 0;
    std::cout << "\n\tVector addition\n";
  }

  temp = a - b;
  if (temp.x != -3.0 || temp.y != 2.0 || temp.z != 3.0) {
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

  if (a.x != 0.0 || a.y != 1.0 || a.z != 0.0) {
    pass = 0;
    std::cout << "\n\tNormal init\n";
  }

  if (b.x != -1.0 || b.y != 0.0 || b.z != 0.0) {
    pass = 0;
    std::cout << "\n\tNormal init\n";
  }

  temp = c + d;
  if (temp.x != 1.0 || temp.y != 0.0 || temp.z != 0.0) {
    pass = 0;
    std::cout << "\n\tNormal addition\n";
  }

  temp = b - c;
  if (temp.x != -1.0 || temp.y != 0.0 || temp.z != 0.0) {
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
  Vector dir = Vector(0, 0, -1);
  Ellipsoid e = Ellipsoid(origin, 1.0, f);
  Ray r = Ray(start, dir, 0.0, 20);
  Ray r2 = Ray(origin, dir, -4.0, 20);
  LocalGeo temp = LocalGeo(); // not static
  Point p = Point(); // not static
  Normal n = Normal(); // not static

  n = e.getNormalAtPoint(a);
  if(n.x != 0.0 || n.y != 1.0 || n.z != 0.0) {
    pass = 0;
    std::cout << "\t\nGet Normal at Point";
  }

  if(e.intersection(r, &temp)) {
    p = temp.getPoint();
    n = temp.getNormal();
    if (p.x != 0.0 || p.y != 0.0 || p.z != 1.0 || n.x != 0.0 || n.y != 0.0 || n.z != 1.0 ) {
      pass = 0;
      std::cout << "\n\tSimple intersection\n";
    }
  } else {
    pass = 0;
    std::cout << "\n\tSimple intersection\n";
  }

  if (e.intersection(r2, &temp)) {
    p = temp.getPoint();
    n = temp.getNormal();
    if (p.x != 0.0 || p.y != 0.0 || p.z != -1.0 || n.x != 0.0 || n.y != 0.0 || n.z != -1.0 ) {
      pass = 0;
      std::cout << "\n\tIntersection from inside sphere\n";
    }
  } else {
    pass = 0;
    std::cout << "\n\tIntersection from inside sphere\n";
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
  if(n.x != 0.0 || n.y != 0.0 || n.z != -1.0) {
    std::cout << n.x;
    std::cout << n.y;
    std::cout << n.z;
    pass = 0;
    std::cout << "\n\tGet Normal";
  }

  if(e.intersection(r, &temp)) {
    p = temp.getPoint();
    n = temp.getNormal();
    if (p.x != 0.0 || p.y != 0.0 || p.z != 0.0 || n.x != 0.0 || n.y != 0.0 || n.z != -1.0 ) {
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

  Point a = Point(1.0, 2.0, 3.0);
  Point b = Point(4.0, 5.0, 6.0);
  Point c = Point(7.0, 8.0, 9.0);
  Point c1 = Point(7.0, 8.0, 10.0);

  Point d = Point(102, 126, 150);
  Point e = Point(30, 36, 42);
  Point f = Point(66, 81, 96);

  Point g = Point(-2.0/3.0f, -4.0/3.0f, 1);
  Point h = Point(-2.0/3.0f, 11.0/3.0f, -2);
  Point i = Point(1, -2, 1);

  Point j = Point(1.0, 4.0, 7.0);
  Point k = Point(2.0, 5.0, 8.0);
  Point l = Point(3.0, 6.0, 9.0);

  Matrix id = Matrix();
  Matrix m1 = Matrix(a, b, c);
  Matrix m1det = Matrix(a, b, c1);
  Matrix m1detinv = Matrix(g, h, i);
  Matrix m2 = Matrix(c, a, b);
  Matrix m3 = Matrix(j, k, l);
  Matrix m1m2 = Matrix(d, e, f);
  Matrix temp = Matrix();

  if (!(id == temp)) {
    pass = 0;
    std::cout << "\n\t== fail";
  }

  temp = m1 * m2;
  if (!(temp == m1m2)) {
    pass = 0;
    std::cout << "\n\tMatrix multiply";
  }

  if (m1det.determinant() != -3.0) {
    pass = 0; std::cout<< "\n\tdeterminant";
  }

  temp = m1det.invert();
  if (!(temp == m1detinv)) {
    pass = 0;
    std::cout << "\n\tinvert";
  }


  return pass;
};


