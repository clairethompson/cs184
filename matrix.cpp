#include "matrix.h"
#include "vector.h"
#include <iostream>
#include <math.h>

Matrix::Matrix() {
  // matrix[down][across]
  this->mat[0][0] = 1.0;
  this->mat[1][0] = 0.0;
  this->mat[2][0] = 0.0;
  this->mat[3][0] = 0.0;

  this->mat[0][1] = 0.0;
  this->mat[1][1] = 1.0;
  this->mat[2][1] = 0.0;
  this->mat[3][1] = 0.0;

  this->mat[0][2] = 0.0;
  this->mat[1][2] = 0.0;
  this->mat[2][2] = 1.0;
  this->mat[3][2] = 0.0;

  this->mat[0][3] = 0.0;
  this->mat[1][3] = 0.0;
  this->mat[2][3] = 0.0;
  this->mat[3][3] = 1.0;
};

Matrix::Matrix(float a[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      this->mat[i][j] = a[i][j];
    }
  }
};

Matrix::Matrix(Point a, Point b, Point c) {
  this->mat[0][0] = a.x;
  this->mat[1][0] = a.y;
  this->mat[2][0] = a.z;
  this->mat[3][0] = 0.0;

  this->mat[0][1] = b.x;
  this->mat[1][1] = b.y;
  this->mat[2][1] = b.z;
  this->mat[3][1] = 0.0;

  this->mat[0][2] = c.x;
  this->mat[1][2] = c.y;
  this->mat[2][2] = c.z;
  this->mat[3][2] = 0.0;

  this->mat[0][3] = 0.0;
  this->mat[1][3] = 0.0;
  this->mat[2][3] = 0.0;
  this->mat[3][3] = 1.0;
};

Matrix::Matrix(Point cen, float rad) {
  this->mat[0][0] = rad;
  this->mat[1][0] = 0.0;
  this->mat[2][0] = 0.0;
  this->mat[3][0] = 0.0;

  this->mat[0][1] = 0.0;
  this->mat[1][1] = rad;
  this->mat[2][1] = 0.0;
  this->mat[3][1] = 0.0;

  this->mat[0][2] = 0.0;
  this->mat[1][2] = 0.0;
  this->mat[2][2] = rad;
  this->mat[3][2] = 0.0;

  this->mat[0][3] = cen.getX();
  this->mat[1][3] = cen.getY();
  this->mat[2][3] = cen.getZ();
  this->mat[3][3] = 1.0;
};

Matrix::Matrix(float a, float b, float c, int type) {
  // type 1 is translation
  // type 2 is rotation
  // type 3 is scaling 
  if (type == 1) {
    /* Translation Matrix */
    this->mat[0][0] = 1.0;
    this->mat[1][0] = 0.0;
    this->mat[2][0] = 0.0;
    this->mat[3][0] = 0.0;

    this->mat[0][1] = 0.0;
    this->mat[1][1] = 1.0;
    this->mat[2][1] = 0.0;
    this->mat[3][1] = 0.0;

    this->mat[0][2] = 0.0;
    this->mat[1][2] = 0.0;
    this->mat[2][2] = 1.0;
    this->mat[3][2] = 0.0;

    this->mat[0][3] = a;
    this->mat[1][3] = b;
    this->mat[2][3] = c;
    this->mat[3][3] = 1.0;

  } else if (type == 2) {
    /* Rotation Matrix */
    Vector r = Vector(a, b, c);
    float theta = r.getLength() * (M_PI/180.0);
    r.normalize();

    Point p1 = Point(0.0, r.z, -r.y);
    Point p2 = Point(-r.z, 0.0, r.x);
    Point p3 = Point(r.y, -r.x, 0.0);
    Matrix rx = Matrix(p1, p2, p3);

    Point p4 = Point(r.x*r.x, r.y*r.x, r.z*r.x);
    Point p5 = Point(r.x*r.y, r.y*r.y, r.z*r.y);
    Point p6 = Point(r.x*r.z, r.y*r.z, r.z*r.z);
    Matrix rrt = Matrix(p4, p5, p6);

    Matrix rxrx = rx * rx;

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        this->mat[i][j] = rrt.getValue(i, j) + sin(theta)*rx.getValue(i, j) - cos(theta)*rxrx.getValue(i, j);
      }
    }

    this->mat[3][0] = 0.0;
    this->mat[3][1] = 0.0;
    this->mat[3][2] = 0.0;

    this->mat[0][3] = 0.0;
    this->mat[1][3] = 0.0;
    this->mat[2][3] = 0.0;
    this->mat[3][3] = 1.0;

  } else if (type == 3) {
    /* Scaling Matrix */
    this->mat[0][0] = a;
    this->mat[1][0] = 0.0;
    this->mat[2][0] = 0.0;
    this->mat[3][0] = 0.0;

    this->mat[0][1] = 0.0;
    this->mat[1][1] = b;
    this->mat[2][1] = 0.0;
    this->mat[3][1] = 0.0;

    this->mat[0][2] = 0.0;
    this->mat[1][2] = 0.0;
    this->mat[2][2] = c;
    this->mat[3][2] = 0.0;

    this->mat[0][3] = 0.0;
    this->mat[1][3] = 0.0;
    this->mat[2][3] = 0.0;
    this->mat[3][3] = 1.0;
  } else {
    // THROW ERROR
  }
};

float Matrix::getValue(int a, int b) {
    return this->mat[a][b];
}

void Matrix::setValue(int a, int b, float c) {
    this->mat[a][b] = c;
}

Matrix Matrix::operator*(Matrix m1) {
  // int sum = 0;
  // float a[4][4];
  // for (int c = 0; c < 4; c++) {
  //   for (int d = 0; d < 4; d++) {
  //     for (int k = 0; k < 4; k++) {
  //       sum = sum + mat[c][k]*m1.getValue(k, d);
  //     }

  //     a[c][d] = sum;
  //     sum = 0;
  //   }
  // }
  // return Matrix(a);

  Matrix m2 = Matrix();

  m2.setValue(0, 0, (this->mat[0][0] * m1.getValue(0, 0) + this->mat[0][1] * m1.getValue(1, 0) + this->mat[0][2] * m1.getValue(2, 0) + this->mat[0][3] * m1.getValue(3,0)));
  m2.setValue(0, 1, (this->mat[0][0] * m1.getValue(0, 1) + this->mat[0][1] * m1.getValue(1, 1) + this->mat[0][2] * m1.getValue(2, 1) + this->mat[0][3] * m1.getValue(3,1)));
  m2.setValue(0, 2, (this->mat[0][0] * m1.getValue(0, 2) + this->mat[0][1] * m1.getValue(1, 2) + this->mat[0][2] * m1.getValue(2, 2) + this->mat[0][3] * m1.getValue(3,2)));
  m2.setValue(0, 3, (this->mat[0][0] * m1.getValue(0, 3) + this->mat[0][1] * m1.getValue(1, 3) + this->mat[0][2] * m1.getValue(2, 3) + this->mat[0][3] * m1.getValue(3,3)));
  m2.setValue(1, 0, (this->mat[1][0] * m1.getValue(0, 0) + this->mat[1][1] * m1.getValue(1, 0) + this->mat[1][2] * m1.getValue(2, 0) + this->mat[1][3] * m1.getValue(3,0)));
  m2.setValue(1, 1, (this->mat[1][0] * m1.getValue(0, 1) + this->mat[1][1] * m1.getValue(1, 1) + this->mat[1][2] * m1.getValue(2, 1) + this->mat[1][3] * m1.getValue(3,1)));
  m2.setValue(1, 2, (this->mat[1][0] * m1.getValue(0, 2) + this->mat[1][1] * m1.getValue(1, 2) + this->mat[1][2] * m1.getValue(2, 2) + this->mat[1][3] * m1.getValue(3,2)));
  m2.setValue(1, 3, (this->mat[1][0] * m1.getValue(0, 3) + this->mat[1][1] * m1.getValue(1, 3) + this->mat[1][2] * m1.getValue(2, 3) + this->mat[1][3] * m1.getValue(3,3)));
  m2.setValue(2, 0, (this->mat[2][0] * m1.getValue(0, 0) + this->mat[2][1] * m1.getValue(1, 0) + this->mat[2][2] * m1.getValue(2, 0) + this->mat[2][3] * m1.getValue(3,0)));
  m2.setValue(2, 1, (this->mat[2][0] * m1.getValue(0, 1) + this->mat[2][1] * m1.getValue(1, 1) + this->mat[2][2] * m1.getValue(2, 1) + this->mat[2][3] * m1.getValue(3,1)));
  m2.setValue(2, 2, (this->mat[2][0] * m1.getValue(0, 2) + this->mat[2][1] * m1.getValue(1, 2) + this->mat[2][2] * m1.getValue(2, 2) + this->mat[2][3] * m1.getValue(3,2)));
  m2.setValue(2, 3, (this->mat[2][0] * m1.getValue(0, 3) + this->mat[2][1] * m1.getValue(1, 3) + this->mat[2][2] * m1.getValue(2, 3) + this->mat[2][3] * m1.getValue(3,3)));
  m2.setValue(3, 0, (this->mat[3][0] * m1.getValue(0, 0) + this->mat[3][1] * m1.getValue(1, 0) + this->mat[3][2] * m1.getValue(2, 0) + this->mat[3][3] * m1.getValue(3,0)));
  m2.setValue(3, 1, (this->mat[3][0] * m1.getValue(0, 1) + this->mat[3][1] * m1.getValue(1, 1) + this->mat[3][2] * m1.getValue(2, 1) + this->mat[3][3] * m1.getValue(3,1)));
  m2.setValue(3, 2, (this->mat[3][0] * m1.getValue(0, 2) + this->mat[3][1] * m1.getValue(1, 2) + this->mat[3][2] * m1.getValue(2, 2) + this->mat[3][3] * m1.getValue(3,2)));
  m2.setValue(3, 3, (this->mat[3][0] * m1.getValue(0, 3) + this->mat[3][1] * m1.getValue(1, 3) + this->mat[3][2] * m1.getValue(2, 3) + this->mat[3][3] * m1.getValue(3,3)));

  return m2;
};
// [dpwn][across]
Vector Matrix::operator*(Vector v) {
  float a = v.getX() * getValue(0, 0) + v.getY() * getValue(0, 1) + v.getZ() * getValue(0, 2);
  float b = v.getX() * getValue(1, 0) + v.getY() * getValue(1, 1) + v.getZ() * getValue(1, 2);
  float c = v.getX() * getValue(2, 0) + v.getY() * getValue(2, 1) + v.getZ() * getValue(2, 2);
  return Vector(a, b, c);
}

Point Matrix::operator*(Point p) {
  float a = p.getX() * getValue(0, 0) + p.getY() * getValue(0, 1) + p.getZ() * getValue(0, 2) + getValue(0, 3);
  float b = p.getX() * getValue(1, 0) + p.getY() * getValue(1, 1) + p.getZ() * getValue(1, 2) + getValue(1, 3);
  float c = p.getX() * getValue(2, 0) + p.getY() * getValue(2, 1) + p.getZ() * getValue(2, 2) + getValue(2, 3);
  float d = getValue(3, 0) + getValue(3, 1) + getValue(3, 2) + getValue(3, 3);

  return Point(a/d, b/d, c/d);

}

void Matrix::operator=(Matrix m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      this->mat[i][j] = m.getValue(i, j);
    }
  }
}

float Matrix::determinant() {
    float det =
          this->mat[0][0]*this->mat[1][1]*this->mat[2][2]*this->mat[3][3]
        + this->mat[0][0]*this->mat[1][2]*this->mat[2][3]*this->mat[3][1]
        + this->mat[0][0]*this->mat[1][3]*this->mat[2][1]*this->mat[3][2]

        + this->mat[0][1]*this->mat[1][0]*this->mat[2][3]*this->mat[3][2]
        + this->mat[0][1]*this->mat[1][2]*this->mat[2][0]*this->mat[3][3]
        + this->mat[0][1]*this->mat[1][3]*this->mat[2][2]*this->mat[3][0]

        + this->mat[0][2]*this->mat[1][0]*this->mat[2][1]*this->mat[3][3]
        + this->mat[0][2]*this->mat[1][1]*this->mat[2][3]*this->mat[3][0]
        + this->mat[0][2]*this->mat[1][3]*this->mat[2][0]*this->mat[3][1]

        + this->mat[0][3]*this->mat[1][0]*this->mat[2][2]*this->mat[3][1]
        + this->mat[0][3]*this->mat[1][1]*this->mat[2][0]*this->mat[3][2]
        + this->mat[0][3]*this->mat[1][2]*this->mat[2][1]*this->mat[3][0]

        - this->mat[0][0]*this->mat[1][1]*this->mat[2][3]*this->mat[3][2]
        - this->mat[0][0]*this->mat[1][2]*this->mat[2][1]*this->mat[3][3]
        - this->mat[0][0]*this->mat[1][3]*this->mat[2][2]*this->mat[3][1]

        - this->mat[0][1]*this->mat[1][0]*this->mat[2][2]*this->mat[3][3]
        - this->mat[0][1]*this->mat[1][2]*this->mat[2][3]*this->mat[3][0]
        - this->mat[0][1]*this->mat[1][3]*this->mat[2][0]*this->mat[3][2]

        - this->mat[0][2]*this->mat[1][0]*this->mat[2][3]*this->mat[3][1]
        - this->mat[0][2]*this->mat[1][1]*this->mat[2][0]*this->mat[3][3]
        - this->mat[0][2]*this->mat[1][3]*this->mat[2][1]*this->mat[3][0]

        - this->mat[0][3]*this->mat[1][0]*this->mat[2][1]*this->mat[3][2]
        - this->mat[0][3]*this->mat[1][1]*this->mat[2][2]*this->mat[3][0]
        - this->mat[0][3]*this->mat[1][2]*this->mat[2][0]*this->mat[3][1];

    return det;
}


Matrix Matrix::invert() {
  float det = determinant();
  if (det != 0) {

    float b00 =   this->mat[1][1]*this->mat[2][2]*this->mat[3][3]
                + this->mat[1][2]*this->mat[2][3]*this->mat[3][1]
                + this->mat[1][3]*this->mat[2][1]*this->mat[3][2]
                - this->mat[1][1]*this->mat[2][3]*this->mat[3][2]
                - this->mat[1][2]*this->mat[2][1]*this->mat[3][3]
                - this->mat[1][3]*this->mat[2][2]*this->mat[3][1];

    float b01 =   this->mat[0][1]*this->mat[2][3]*this->mat[3][2]
                + this->mat[0][2]*this->mat[2][1]*this->mat[3][3]
                + this->mat[0][3]*this->mat[2][2]*this->mat[3][1]
                - this->mat[0][1]*this->mat[2][2]*this->mat[3][3]
                - this->mat[0][2]*this->mat[2][3]*this->mat[3][1]
                - this->mat[0][3]*this->mat[2][1]*this->mat[3][2];

    float b02 =   this->mat[0][1]*this->mat[1][2]*this->mat[3][3]
                + this->mat[0][2]*this->mat[1][3]*this->mat[3][1]
                + this->mat[0][3]*this->mat[1][1]*this->mat[3][2]
                - this->mat[0][1]*this->mat[1][3]*this->mat[3][2]
                - this->mat[0][2]*this->mat[1][1]*this->mat[3][3]
                - this->mat[0][3]*this->mat[1][2]*this->mat[3][1];

    float b03 =   this->mat[0][1]*this->mat[1][3]*this->mat[2][2]
                + this->mat[0][2]*this->mat[1][1]*this->mat[2][3]
                + this->mat[0][3]*this->mat[1][2]*this->mat[2][1]
                - this->mat[0][1]*this->mat[1][2]*this->mat[2][3]
                - this->mat[0][2]*this->mat[1][3]*this->mat[2][1]
                - this->mat[0][3]*this->mat[1][1]*this->mat[2][2];

    float b10 =   this->mat[1][0]*this->mat[2][3]*this->mat[3][2]
                + this->mat[1][2]*this->mat[2][0]*this->mat[3][3]
                + this->mat[1][3]*this->mat[2][2]*this->mat[3][0]
                - this->mat[1][0]*this->mat[2][2]*this->mat[3][3]
                - this->mat[1][2]*this->mat[2][3]*this->mat[3][0]
                - this->mat[1][3]*this->mat[2][0]*this->mat[3][2];

    float b11 =   this->mat[0][0]*this->mat[2][2]*this->mat[3][3]
                + this->mat[0][2]*this->mat[2][3]*this->mat[3][0]
                + this->mat[0][3]*this->mat[2][0]*this->mat[3][2]
                - this->mat[0][0]*this->mat[2][3]*this->mat[3][2]
                - this->mat[0][2]*this->mat[2][0]*this->mat[3][3]
                - this->mat[0][3]*this->mat[2][2]*this->mat[3][0];

    float b12 =   this->mat[0][0]*this->mat[1][3]*this->mat[3][2]
                + this->mat[0][2]*this->mat[1][0]*this->mat[3][3]
                + this->mat[0][3]*this->mat[1][2]*this->mat[3][0]
                - this->mat[0][0]*this->mat[1][2]*this->mat[3][3]
                - this->mat[0][2]*this->mat[1][3]*this->mat[3][0]
                - this->mat[0][3]*this->mat[1][0]*this->mat[3][2];

    float b13 =   this->mat[0][0]*this->mat[1][2]*this->mat[2][3]
                + this->mat[0][2]*this->mat[1][3]*this->mat[2][0]
                + this->mat[0][3]*this->mat[1][0]*this->mat[2][2]
                - this->mat[0][0]*this->mat[1][3]*this->mat[2][2]
                - this->mat[0][2]*this->mat[1][0]*this->mat[2][3]
                - this->mat[0][3]*this->mat[1][2]*this->mat[2][0];

    float b20 =   this->mat[1][0]*this->mat[2][1]*this->mat[3][3]
                + this->mat[1][1]*this->mat[2][3]*this->mat[3][0]
                + this->mat[1][3]*this->mat[2][0]*this->mat[3][1]
                - this->mat[1][0]*this->mat[2][3]*this->mat[3][1]
                - this->mat[1][1]*this->mat[2][0]*this->mat[3][3]
                - this->mat[1][3]*this->mat[2][1]*this->mat[3][0];

    float b21 =   this->mat[0][0]*this->mat[2][3]*this->mat[3][1]
                + this->mat[0][1]*this->mat[2][0]*this->mat[3][3]
                + this->mat[0][3]*this->mat[2][1]*this->mat[3][0]
                - this->mat[0][0]*this->mat[2][1]*this->mat[3][3]
                - this->mat[0][1]*this->mat[2][3]*this->mat[3][0]
                - this->mat[0][3]*this->mat[2][0]*this->mat[3][1];

    float b22 =   this->mat[0][0]*this->mat[1][1]*this->mat[3][3]
                + this->mat[0][1]*this->mat[1][3]*this->mat[3][0]
                + this->mat[0][3]*this->mat[1][0]*this->mat[3][1]
                - this->mat[0][0]*this->mat[1][3]*this->mat[3][1]
                - this->mat[0][1]*this->mat[1][0]*this->mat[3][3]
                - this->mat[0][3]*this->mat[1][1]*this->mat[3][0];

    float b23 =   this->mat[0][0]*this->mat[1][3]*this->mat[2][1]
                + this->mat[0][1]*this->mat[1][0]*this->mat[2][3]
                + this->mat[0][3]*this->mat[1][1]*this->mat[2][0]
                - this->mat[0][0]*this->mat[1][1]*this->mat[2][3]
                - this->mat[0][1]*this->mat[1][3]*this->mat[2][0]
                - this->mat[0][3]*this->mat[1][0]*this->mat[2][1];

    float b30 =   this->mat[1][0]*this->mat[2][2]*this->mat[3][1]
                + this->mat[1][1]*this->mat[2][0]*this->mat[3][2]
                + this->mat[1][2]*this->mat[2][1]*this->mat[3][0]
                - this->mat[1][0]*this->mat[2][1]*this->mat[3][2]
                - this->mat[1][1]*this->mat[2][2]*this->mat[3][0]
                - this->mat[1][2]*this->mat[2][0]*this->mat[3][1];

    float b31 =   this->mat[0][0]*this->mat[2][1]*this->mat[3][2]
                + this->mat[0][1]*this->mat[2][2]*this->mat[3][0]
                + this->mat[0][2]*this->mat[2][0]*this->mat[3][1]
                - this->mat[0][0]*this->mat[2][2]*this->mat[3][1]
                - this->mat[0][1]*this->mat[2][0]*this->mat[3][2]
                - this->mat[0][2]*this->mat[2][1]*this->mat[3][0];

    float b32 =   this->mat[0][0]*this->mat[1][2]*this->mat[3][1]
                + this->mat[0][1]*this->mat[1][0]*this->mat[3][2]
                + this->mat[0][2]*this->mat[1][1]*this->mat[3][0]
                - this->mat[0][0]*this->mat[1][1]*this->mat[3][2]
                - this->mat[0][1]*this->mat[1][2]*this->mat[3][0]
                - this->mat[0][2]*this->mat[1][0]*this->mat[3][1];

    float b33 =   this->mat[0][0]*this->mat[1][1]*this->mat[2][2]
                + this->mat[0][1]*this->mat[1][2]*this->mat[2][0]
                + this->mat[0][2]*this->mat[1][0]*this->mat[2][1]
                - this->mat[0][0]*this->mat[1][2]*this->mat[2][1]
                - this->mat[0][1]*this->mat[1][0]*this->mat[2][2]
                - this->mat[0][2]*this->mat[1][1]*this->mat[2][0];

    Matrix inverted = Matrix();
    inverted.setValue(0, 0, b00/det);
    inverted.setValue(0, 1, b01/det);
    inverted.setValue(0, 2, b02/det);
    inverted.setValue(0, 3, b03/det);
    inverted.setValue(1, 0, b10/det);
    inverted.setValue(1, 1, b11/det);
    inverted.setValue(1, 2, b12/det);
    inverted.setValue(1, 3, b13/det);
    inverted.setValue(2, 0, b20/det);
    inverted.setValue(2, 1, b21/det);
    inverted.setValue(2, 2, b22/det);
    inverted.setValue(2, 3, b23/det);
    inverted.setValue(3, 0, b30/det);
    inverted.setValue(3, 1, b31/det);
    inverted.setValue(3, 2, b32/det);
    inverted.setValue(3, 3, b33/det);

    return inverted;

    } else {
        // THROW ERROR
    }
}

Matrix Matrix::transpose() {
  Matrix ret = Matrix();

  for (int i = 0; i < 4; i ++) {
    for (int j = 0; j < 4; j++) {
      ret.setValue(i, j, this->mat[j][i]);
    }
  }
  
  return ret;
}

bool Matrix::operator==(Matrix m) {
  bool pass = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if(this->mat[i][j] != m.getValue(i, j)) {
        std::cout << "\n";
        std::cout << "\n";
        std::cout << i;
        std::cout << "\n";
        std::cout << j;
        std::cout << "\n";
        std::cout << this->mat[i][j];
        std::cout << "\n";
        std::cout << m.getValue(i, j);
        pass = 0;
      }
    }
  }
  return pass;
}

void Matrix::print() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << this->mat[i][j];
      std::cout << ",";
    }
    std::cout << "\n";
  }
  std::cout<< "\n";
}

