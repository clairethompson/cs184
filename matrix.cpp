#include "matrix.h"
#include "vector.h"

Matrix::Matrix() {
    this->mat[0][0] = 1.0;
    this->mat[0][1] = 0.0;
    this->mat[0][2] = 0.0;
    this->mat[0][3] = 0.0;

    this->mat[1][0] = 0.0;
    this->mat[1][1] = 1.0;
    this->mat[1][2] = 0.0;
    this->mat[1][3] = 0.0;

    this->mat[2][0] = 0.0;
    this->mat[2][1] = 0.0;
    this->mat[2][2] = 1.0;
    this->mat[2][3] = 0.0;

    this->mat[3][0] = 0.0;
    this->mat[3][1] = 0.0;
    this->mat[3][2] = 0.0;
    this->mat[3][3] = 1.0;
};

Matrix::Matrix(Point a, Point b, Point c) {
    this->mat[0][0] = a.x;
    this->mat[0][1] = a.y;
    this->mat[0][2] = a.z;
    this->mat[0][3] = 0.0;

    this->mat[1][0] = b.x;
    this->mat[1][1] = b.y;
    this->mat[1][2] = b.z;
    this->mat[1][3] = 0.0;

    this->mat[2][0] = c.x;
    this->mat[2][1] = c.y;
    this->mat[2][2] = c.z;
    this->mat[2][3] = 0.0;

    this->mat[3][0] = 0.0;
    this->mat[3][1] = 0.0;
    this->mat[3][2] = 0.0;
    this->mat[3][3] = 1.0;
};

Matrix::Matrix(float a, float b, float c, int type) {
  if (type == 1 || type == 3) {
    /* Translation and Scaling Matrices */
    this->mat[0][0] = 1.0;
    this->mat[0][1] = 0.0;
    this->mat[0][2] = 0.0;
    this->mat[0][3] = 0.0;

    this->mat[1][0] = 0.0;
    this->mat[1][1] = 1.0;
    this->mat[1][2] = 0.0;
    this->mat[1][3] = 0.0;

    this->mat[2][0] = 0.0;
    this->mat[2][1] = 0.0;
    this->mat[2][2] = 1.0;
    this->mat[2][3] = 0.0;

    this->mat[3][0] = a;
    this->mat[3][1] = b;
    this->mat[3][2] = c;
    this->mat[3][3] = 1.0;

  } else if (type == 2) {
    /* Rotation Matrix */
    Vector exp = Vector(a, b, c);
    float theta = exp.getLength();
    exp.normalize();
    // MORE STUFF HERE
  } else {
    // THROW ERROR
    // exit(0);
  }
};