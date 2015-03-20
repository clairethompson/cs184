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
  std::cout << "in add transformation\n";
  m.print();
  // std::cout<< "\n";
  // temp.print();
  // std::cout<< "\n";
  // inv.print();
  // std::cout<< "\n";

  this->inv = temp * this->inv;
  // std::cout << "after\n";
  // inv.print();
  // std::cout<< "\n";

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