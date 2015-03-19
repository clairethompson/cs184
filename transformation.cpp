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
  this->trans = this->trans * m;
  this->inv = this->trans.invert();
};

void Transformation::clear() {
  this->trans = Matrix();
  this->inv = Matrix();
};

Matrix Transformation::getTrans() {
  return this->trans;
}