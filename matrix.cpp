#include "matrix.h"
#include "vector.h"
#include <iostream>
#include <math.h>

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

Matrix::Matrix(float a[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      this->mat[i][j] = a[i][j];
    }
  }
};

Matrix::Matrix(Point a, Point b, Point c) {
  this->mat[0][0] = a.getX();
  this->mat[0][1] = a.getY();
  this->mat[0][2] = a.getZ();
  this->mat[0][3] = 0.0;

  this->mat[1][0] = b.getX();
  this->mat[1][1] = b.getY();
  this->mat[1][2] = b.getZ();
  this->mat[1][3] = 0.0;

  this->mat[2][0] = c.getX();
  this->mat[2][1] = c.getY();
  this->mat[2][2] = c.getZ();
  this->mat[2][3] = 0.0;

  this->mat[3][0] = 0.0;
  this->mat[3][1] = 0.0;
  this->mat[3][2] = 0.0;
  this->mat[3][3] = 1.0;
};

Matrix::Matrix(Point cen, float rad) {
  float a[4][4] = {{rad, 0.0, 0.0, 0.0}, {0, rad, 0, 0}, {0, 0, rad, 0}, {cen.getX(), cen.getY(), cen.getZ(), 1.0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      this->mat[i][j] = a[i][j];
    }
  }
};

Matrix::Matrix(float a, float b, float c, int type) {
  // type 1 is translation
  // type 2 is rotation
  // type 3 is scaling 
  if (type == 1) {
    /* Translation Matrix */
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
    Vector r = Vector(a, b, c);
    float theta = r.getLength();
    r.normalize();

    Point p1 = Point(0.0, r.z, -r.y);
    Point p2 = Point(-r.z, 0.0, r.x);
    Point p3 = Point(r.y, -r.x, 0.0);
    Matrix rx = Matrix(p1, p2, p3);

    Point p4 = Point(r.x*r.x, r.y*r.x, r.z*r.x);
    Point p5 = Point(r.x*r.y, r.y*r.y, r.z*r.y);
    Point p6 = Point(r.x*r.z, r.y*r.z, r.z*r.z);
    Matrix rrt = Matrix(p4, p5, p6);

    Matrix rxrx = rx*rx;

    this->mat[0][0] = rrt.getValue(0, 0) + sin(theta)*rx.getValue(0, 0) - cos(theta)*rxrx.getValue(0, 0);
    this->mat[0][1] = rrt.getValue(0, 1) + sin(theta)*rx.getValue(0, 1) - cos(theta)*rxrx.getValue(0, 1);
    this->mat[0][2] = rrt.getValue(0, 2) + sin(theta)*rx.getValue(0, 2) - cos(theta)*rxrx.getValue(0, 2);
    this->mat[0][3] = rrt.getValue(0, 3) + sin(theta)*rx.getValue(0, 3) - cos(theta)*rxrx.getValue(0, 3);
    this->mat[1][0] = rrt.getValue(1, 0) + sin(theta)*rx.getValue(1, 0) - cos(theta)*rxrx.getValue(1, 0);
    this->mat[1][1] = rrt.getValue(1, 1) + sin(theta)*rx.getValue(1, 1) - cos(theta)*rxrx.getValue(1, 1);
    this->mat[1][2] = rrt.getValue(1, 2) + sin(theta)*rx.getValue(1, 2) - cos(theta)*rxrx.getValue(1, 2);
    this->mat[1][3] = rrt.getValue(1, 3) + sin(theta)*rx.getValue(1, 3) - cos(theta)*rxrx.getValue(1, 3);
    this->mat[2][0] = rrt.getValue(2, 0) + sin(theta)*rx.getValue(2, 0) - cos(theta)*rxrx.getValue(2, 0);
    this->mat[2][1] = rrt.getValue(2, 1) + sin(theta)*rx.getValue(2, 1) - cos(theta)*rxrx.getValue(2, 1);
    this->mat[2][2] = rrt.getValue(2, 2) + sin(theta)*rx.getValue(2, 2) - cos(theta)*rxrx.getValue(2, 2);
    this->mat[2][3] = rrt.getValue(2, 3) + sin(theta)*rx.getValue(2, 3) - cos(theta)*rxrx.getValue(2, 3);
    this->mat[3][0] = rrt.getValue(3, 0) + sin(theta)*rx.getValue(3, 0) - cos(theta)*rxrx.getValue(3, 0);
    this->mat[3][1] = rrt.getValue(3, 1) + sin(theta)*rx.getValue(3, 1) - cos(theta)*rxrx.getValue(3, 1);
    this->mat[3][2] = rrt.getValue(3, 2) + sin(theta)*rx.getValue(3, 2) - cos(theta)*rxrx.getValue(3, 2);
    this->mat[3][3] = rrt.getValue(3, 3) + sin(theta)*rx.getValue(3, 3) - cos(theta)*rxrx.getValue(3, 3);

  } else if (type == 3) {
    /* Scaling Matrix */
    this->mat[0][0] = a;
    this->mat[0][1] = 0.0;
    this->mat[0][2] = 0.0;
    this->mat[0][3] = 0.0;

    this->mat[1][0] = 0.0;
    this->mat[1][1] = b;
    this->mat[1][2] = 0.0;
    this->mat[1][3] = 0.0;

    this->mat[2][0] = 0.0;
    this->mat[2][1] = 0.0;
    this->mat[2][2] = c;
    this->mat[2][3] = 0.0;

    this->mat[3][0] = 0.0;
    this->mat[3][1] = 0.0;
    this->mat[3][2] = 0.0;
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
  float ret[4][4];
  float sum;

  for (int c = 0; c < 4; c++) {
    for (int d = 0; d < 4; d++) {
      for (int k = 0; k < 4; k++) {
        sum += this->mat[k][c]*m1.getValue(d, k);
      }
      ret[c][d] = sum;
      sum = 0;
    }
  }
  Matrix m2 = Matrix(ret);
  m2.print();
  return m2.transpose();
};

Matrix Matrix::operator*(float f) {
  float a[4][4];
  for (int i = 0; i < 4; i ++) {
    for (int j = 0; j < 4; j++) {
      a[i][j] = this->mat[i][j] * f;
    }
  }
  return Matrix(a);
}

void Matrix::operator=(Matrix m) {
  for (int i = 0; i < 4; i ++) {
    for (int j = 0; j < 4; j++) {
      this->mat[i][j] = m.getValue(i, j);
    }
  }
}

Vector Matrix::operator*(Vector v) {
  float a = v.getX() * getValue(0, 0) + v.getY() * getValue(1, 0) + v.getZ() * getValue(2, 0) + getValue(3, 0);
  float b = v.getX() * getValue(0, 1) + v.getY() * getValue(1, 1) + v.getZ() * getValue(2, 1) + getValue(3, 1);
  float c = v.getX() * getValue(0, 2) + v.getY() * getValue(1, 2) + v.getZ() * getValue(2, 2) + getValue(3, 2);
  float d = v.getX() * getValue(0, 3) + v.getY() * getValue(1, 3) + v.getZ() * getValue(2, 3) + getValue(3, 3);
  return Vector(a/d, b/d, c/d);
}

float Matrix::determinant() {
    float det =
          this->mat[0][0]*this->mat[1][1]*this->mat[2][2]*this->mat[3][3]
        + this->mat[0][0]*this->mat[2][1]*this->mat[3][2]*this->mat[1][3]
        + this->mat[0][0]*this->mat[3][1]*this->mat[1][2]*this->mat[2][3]

        + this->mat[1][0]*this->mat[0][1]*this->mat[3][2]*this->mat[2][3]
        + this->mat[1][0]*this->mat[2][1]*this->mat[0][2]*this->mat[3][3]
        + this->mat[1][0]*this->mat[3][1]*this->mat[2][2]*this->mat[0][3]

        + this->mat[2][0]*this->mat[0][1]*this->mat[1][2]*this->mat[3][3]
        + this->mat[2][0]*this->mat[1][1]*this->mat[3][2]*this->mat[0][3]
        + this->mat[2][0]*this->mat[3][1]*this->mat[0][2]*this->mat[1][3]

        + this->mat[3][0]*this->mat[0][1]*this->mat[2][2]*this->mat[1][3]
        + this->mat[3][0]*this->mat[1][1]*this->mat[0][2]*this->mat[2][3]
        + this->mat[3][0]*this->mat[2][1]*this->mat[1][2]*this->mat[0][3]

        - this->mat[0][0]*this->mat[1][1]*this->mat[3][2]*this->mat[2][3]
        - this->mat[0][0]*this->mat[2][1]*this->mat[1][2]*this->mat[3][3]
        - this->mat[0][0]*this->mat[3][1]*this->mat[2][2]*this->mat[1][3]

        - this->mat[1][0]*this->mat[0][1]*this->mat[2][2]*this->mat[3][3]
        - this->mat[1][0]*this->mat[2][1]*this->mat[3][2]*this->mat[0][3]
        - this->mat[1][0]*this->mat[3][1]*this->mat[0][2]*this->mat[2][3]

        - this->mat[2][0]*this->mat[0][1]*this->mat[3][2]*this->mat[1][3]
        - this->mat[2][0]*this->mat[1][1]*this->mat[0][2]*this->mat[3][3]
        - this->mat[2][0]*this->mat[3][1]*this->mat[1][2]*this->mat[0][3]

        - this->mat[3][0]*this->mat[0][1]*this->mat[1][2]*this->mat[2][3]
        - this->mat[3][0]*this->mat[1][1]*this->mat[2][2]*this->mat[0][3]
        - this->mat[3][0]*this->mat[2][1]*this->mat[0][2]*this->mat[1][3];

    return det;
}

Matrix Matrix::invert() {
  float det = determinant();
  float inv[4][4];

  inv[0][0] = mat[0][1]  * mat[2][2] * mat[3][3] - 
           mat[0][1]  * mat[3][2] * mat[2][3] - 
           mat[1][2]  * mat[2][1]  * mat[3][3] + 
           mat[1][2]  * mat[3][1]  * mat[2][3] +
           mat[1][3] * mat[2][1]  * mat[3][2] - 
           mat[1][3] * mat[3][1]  * mat[2][2];

  inv[0][1] = -mat[0][1]  * mat[2][2] * mat[3][3] + 
            mat[0][1]  * mat[3][2] * mat[2][3] + 
            mat[0][2]  * mat[2][1]  * mat[3][3] - 
            mat[0][2]  * mat[3][1]  * mat[2][3] - 
            mat[0][3] * mat[2][1]  * mat[3][2] + 
            mat[0][3] * mat[3][1]  * mat[2][2];

  inv[0][2] = mat[0][1]  * mat[1][2] * mat[3][3] - 
           mat[0][1]  * mat[3][2] * mat[1][3] - 
           mat[0][2]  * mat[1][1] * mat[3][3] + 
           mat[0][2]  * mat[3][1] * mat[1][3] + 
           mat[0][3] * mat[1][1] * mat[3][2] - 
           mat[0][3] * mat[3][1] * mat[1][2];

  inv[0][3] = -mat[0][1] * mat[1][2] * mat[2][3] + 
               mat[0][1] * mat[2][2] * mat[1][3] +
               mat[0][2] * mat[1][1] * mat[2][3] - 
               mat[0][2] * mat[2][1] * mat[1][3] - 
               mat[0][3] * mat[1][1] * mat[2][2] + 
               mat[0][3] * mat[2][1] * mat[1][2];

  inv[1][0] = -mat[1][0]  * mat[2][2] * mat[3][3] + 
            mat[1][0]  * mat[3][2] * mat[2][3] + 
            mat[1][2]  * mat[2][0]* mat[3][3] - 
            mat[1][2]  * mat[3][0] * mat[2][3] - 
            mat[1][3] * mat[2][0]* mat[3][2] + 
            mat[1][3] * mat[3][0] * mat[2][2];

  inv[1][1] = mat[0][0]  * mat[2][2] * mat[3][3] - 
           mat[0][0]  * mat[3][2] * mat[2][3] - 
           mat[0][2]  * mat[2][0]* mat[3][3] + 
           mat[0][2]  * mat[3][0] * mat[2][3] + 
           mat[0][3] * mat[2][0]* mat[3][2] - 
           mat[0][3] * mat[3][0] * mat[2][2];

  inv[1][2] = -mat[0][0]  * mat[1][2] * mat[3][3] + 
            mat[0][0]  * mat[3][2] * mat[1][3] + 
            mat[0][2]  * mat[1][0] * mat[3][3] - 
            mat[0][2]  * mat[3][0] * mat[1][3] - 
            mat[0][3] * mat[1][0] * mat[3][2] + 
            mat[0][3] * mat[3][0] * mat[1][2];

  inv[1][3] = mat[0][0]  * mat[1][2] * mat[2][3] - 
            mat[0][0]  * mat[2][2] * mat[1][3] - 
            mat[0][2]  * mat[1][0] * mat[2][3] + 
            mat[0][2]  * mat[2][0]* mat[1][3] + 
            mat[0][3] * mat[1][0] * mat[2][2] - 
            mat[0][3] * mat[2][0]* mat[1][2];

  inv[2][0] = mat[1][0]  * mat[2][1] * mat[3][3] - 
           mat[1][0]  * mat[3][1] * mat[2][3] - 
           mat[1][1]  * mat[2][0]* mat[3][3] + 
           mat[1][1]  * mat[3][0] * mat[2][3] + 
           mat[1][3] * mat[2][0]* mat[3][1] - 
           mat[1][3] * mat[3][0] * mat[2][1];

  inv[2][1] = -mat[0][0]  * mat[2][1] * mat[3][3] + 
            mat[0][0]  * mat[3][1] * mat[2][3] + 
            mat[0][1]  * mat[2][0]* mat[3][3] - 
            mat[0][1]  * mat[3][0] * mat[2][3] - 
            mat[0][3] * mat[2][0]* mat[3][1] + 
            mat[0][3] * mat[3][0] * mat[2][1];

  inv[2][2] = mat[0][0]  * mat[1][1] * mat[3][3] - 
            mat[0][0]  * mat[3][1] * mat[1][3] - 
            mat[0][1]  * mat[1][0] * mat[3][3] + 
            mat[0][1]  * mat[3][0] * mat[1][3] + 
            mat[0][3] * mat[1][0] * mat[3][1] - 
            mat[0][3] * mat[3][0] * mat[1][1];

  inv[2][3] = -mat[0][0]  * mat[1][1] * mat[2][3] + 
             mat[0][0]  * mat[2][1] * mat[1][3] + 
             mat[0][1]  * mat[1][0] * mat[2][3] - 
             mat[0][1]  * mat[2][0]* mat[1][3] - 
             mat[0][3] * mat[1][0] * mat[2][1] + 
             mat[0][3] * mat[2][0]* mat[1][1];

  inv[3][0] = -mat[1][0] * mat[2][1] * mat[3][2] + 
            mat[1][0] * mat[3][1] * mat[2][2] + 
            mat[1][1] * mat[2][0]* mat[3][2] - 
            mat[1][1] * mat[3][0] * mat[2][2] - 
            mat[1][2] * mat[2][0]* mat[3][1] + 
            mat[1][2] * mat[3][0] * mat[2][1];

  inv[3][1] = mat[0][0] * mat[2][1] * mat[3][2] - 
           mat[0][0] * mat[3][1] * mat[2][2] - 
           mat[0][1] * mat[2][0]* mat[3][2] + 
           mat[0][1] * mat[3][0] * mat[2][2] + 
           mat[0][2] * mat[2][0]* mat[3][1] - 
           mat[0][2] * mat[3][0] * mat[2][1];

  inv[3][2] = -mat[0][0] * mat[1][1] * mat[3][2] + 
             mat[0][0] * mat[3][1] * mat[1][2] + 
             mat[0][1] * mat[1][0] * mat[3][2] - 
             mat[0][1] * mat[3][0] * mat[1][2] - 
             mat[0][2] * mat[1][0] * mat[3][1] + 
             mat[0][2] * mat[3][0] * mat[1][1];

  inv[3][3] = mat[0][0] * mat[1][1] * mat[2][2] - 
            mat[0][0] * mat[2][1] * mat[1][2] - 
            mat[0][1] * mat[1][0] * mat[2][2] + 
            mat[0][1] * mat[2][0]* mat[1][2] + 
            mat[0][2] * mat[1][0] * mat[2][1] - 
            mat[0][2] * mat[2][0]* mat[1][1];

  // det = mat[0][0] * inv[0][0] + mat[1][0] * inv[0][1] + mat[2][0]* inv[0][2] + mat[3][0] * inv[0][3];

  Matrix temp = Matrix(inv);

  det = 1.0f / det;
  temp = temp * det;

  std::cout << "in invert\n";
  temp.print();
  std::cout<< det;
  std::cout<< "\n";

  return temp;
}

Matrix Matrix::transpose() {
    Matrix ret = Matrix();

    ret.setValue(0, 0, this->mat[0][0]);
    ret.setValue(1, 0, this->mat[0][1]);
    ret.setValue(2, 0, this->mat[0][2]);
    ret.setValue(3, 0, this->mat[0][3]);

    ret.setValue(0, 1, this->mat[1][0]);
    ret.setValue(1, 1, this->mat[1][1]);
    ret.setValue(2, 1, this->mat[1][2]);
    ret.setValue(3, 1, this->mat[1][3]);

    ret.setValue(0, 2, this->mat[2][0]);
    ret.setValue(1, 2, this->mat[2][1]);
    ret.setValue(2, 2, this->mat[2][2]);
    ret.setValue(3, 2, this->mat[2][3]);

    ret.setValue(0, 3, this->mat[3][0]);
    ret.setValue(1, 3, this->mat[3][1]);
    ret.setValue(2, 3, this->mat[3][2]);
    ret.setValue(3, 3, this->mat[3][3]);

    return ret;
}

bool Matrix::operator==(Matrix m) {
  bool pass = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if(this->mat[i][j] != m.getValue(i, j)) {
        pass = 0;
      }
    }
  }
  if (!pass) {
    print();
    std::cout<< "\n";
    m.print();
  }

  return pass;
}

void Matrix::print() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << this->mat[j][i];
      std::cout << ",";
    }
    std::cout << "\n";
  }
  std::cout<< "\n";
}

