#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"
#include "viewplane.h"

class Camera {
  Point eye;
  ViewPlane vp;

public:
  Camera(Point e, Point llp, Point lrp, Point ulp, Point urp);
  Camera();
  float getHeight();
  float getWidth();
};

#endif