#include "transformation.h"

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
  this->inv = m.invert() * this->inv;
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