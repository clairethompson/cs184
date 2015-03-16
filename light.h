#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"
#include "color.h"

class Light {
  Point p;
  Color intensity;
  float falloff;
public:
  Light(float r, float g, float b);
  Light(float x, float y, float z, float r, float g, float b, float falloff);
  Light(float x, float y, float z, float r, float g, float b);
  Color getIntensity();
  Point getPoint();
  float getFalloff();

};

#endif