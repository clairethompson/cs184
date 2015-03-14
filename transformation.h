#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "matrix.h"

class Transformation {
  Matrix trans;
public:
  Transformation(Matrix m);  
  Transformation();  
  void addTransformation();
  void clear();
};

#endif