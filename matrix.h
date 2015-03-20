#ifndef MATRIX_H
#define MATRIX_H

#include "point.h"
#include "vector.h"

class Matrix {
  float mat[4][4];
public:
  /* Type 1: Translation; Type 2: Rotation; Type 3: Scaling; else: error */
  Matrix(float a, float b, float c, int type);  
  Matrix();
  Matrix(Point a, Point b, Point c);
  Matrix(Point center, float rad); // for spheres
  Matrix(float a[4][4]);
  float getValue(int a, int b);
  void setValue(int a, int b, float c);
  Matrix operator*(Matrix m);
  Matrix operator*(float f);
  void operator=(Matrix m);
  Vector operator*(Vector v);
  bool operator==(Matrix m);
  float determinant();
  Matrix invert();
  Matrix transpose();

  /* FOR DEBUGGIN */
  void print();
};

#endif