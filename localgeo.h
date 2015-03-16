#ifndef LOCALGEO_H
#define LOCALGEO_H

#include "point.h"
#include "normal.h"

class LocalGeo {
  Point p;
  Normal n;

public:
  LocalGeo();
  LocalGeo(Point p, Normal n);
  void setPoint(Point hitP);
  void setNormal(Normal hitN);
  Point getPoint();
  Normal getNormal();
};

#endif