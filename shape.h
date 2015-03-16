#ifndef SHAPE_H
#define SHAPE_H

#include "matrix.h"
#include "BRDF.h"
#include "ray.h"
#include "localgeo.h"

class Shape {
protected:
  BRDF f;
  Matrix m;

public:   
  /* If there is an intersection, return 1 and update l to the intersection point */
  virtual bool intersection(Ray r, LocalGeo* l) = 0;
  BRDF getBRDF() { return this->f; }
};

#endif
