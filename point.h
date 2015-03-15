#ifndef POINT_H
#define POINT_H

class Point {
  public:
  	float x, y, z;

  public:
    Point (float a, float b, float c);
  	Point ();

  	/* Getter Functions */
  	float getX();
  	float getY();
  	float getZ();
};

#endif