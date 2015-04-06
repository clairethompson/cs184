#ifndef VIEWPLANE_H
#define VIEWPLANE_H

#include "point.h"

class ViewPlane {
  float width, height;
  Point ll, lr, ul, ur;

public:
  /* Init ViewPlane */
  ViewPlane(Point ll, Point lr, Point ul, Point ur);
  ViewPlane();
  float getHeight();
  float getWidth();
  Point getLL();
  Point getLR();
  Point getUL();
  Point getUR();
};

#endif