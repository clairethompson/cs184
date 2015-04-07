#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "matrix.h"

class Transformation {
  Matrix trans, inv;
public:
  Transformation(Matrix m);  
  Transformation();  
  void addTransformation(Matrix m);
  void clear();
  Matrix getTrans();
  Matrix getInv();
};

#endif