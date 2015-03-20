#include "transformation.h"
#include <iostream>

Transformation::Transformation(Matrix m) {
    this->trans = m;
    this->inv = m.invert();
};

Transformation::Transformation() {
    this->trans = Matrix();
    this->inv = Matrix();
};

void Transformation::addTransformation(Matrix m) {
  this->trans = m * this->trans;
  Matrix temp = m.invert();
  std::cout << "in add transformation\n";
  temp.print();
  std::cout<< "\n";
  this->inv = temp * this->inv;
};

void Transformation::clear() {
  this->trans = Matrix();
  this->inv = Matrix();
};

Matrix Transformation::getTrans() {
  return this->trans;
}

Matrix Transformation::getInv() {
  return this->inv;
}