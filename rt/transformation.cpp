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
  this->trans = this->trans * m;
  Matrix temp = m.invert();
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