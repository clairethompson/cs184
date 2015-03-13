#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

class Transformation {
  Matrix trans;
public:
  Transformation(Matrix m);  
  Transformation();  
  addTransformation();
  clear();
};

#endif